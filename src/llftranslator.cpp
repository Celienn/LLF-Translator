#include "llftranslator.h"

LLFTranslator::LLFTranslator()
{
    config = loadConfig();
    udpWorker = new UDPWorker(this);
    w = new MainWindow(this);
    w->show();
    w->setWindowTitle("LLF Translator");
}

LLFTranslator::~LLFTranslator()
{
    delete udpWorker;
    delete w;
}

/**
 * @brief Lit une variable de simulation et déclenche un rappel avec sa valeur.
 * 
 * @param dataref Pointeur vers la classe Dataref qui contient les informations sur la variable de simulation.
 * @param type Le type de données de la variable de simulation.
 * @param callback Une fonction de rappel qui est appelée quand la variable de simulation est reçut et prend en argument sa valeur.
 * @param frequency La fréquence à laquelle la variable de simulation sera envoyer ( vers FlightSim ).
 * 
 * @return void
 * 
 * Si la connexion à la simulation n'est pas établie, cette fonction se stop immédiatement.
 * Sinon, elle ajoute la variable de simulation à la définition des données et demande à SimConnect de fournir ses données.
 * Si l'ajout à la définition des données échoue, un message d'erreur est enregistré.
 */
void LLFTranslator::readVar(Dataref* dataref, SIMCONNECT_DATATYPE type, function<void(double)> callback, int frequency){
    if (!isConnected()) return;

    DWORD definition = nextDefinitionId++;
    dataref->definitionId = definition;
    dataref->callback = [callback, this](SIMCONNECT_RECV_SIMOBJECT_DATA *pObjData){
        double* data = (double*)&pObjData->dwData;
        callback(*data);
    };
    
    variables[dataref->requestId] = dataref;

    HRESULT hr = SimConnect_AddToDataDefinition(hSimConnect, definition, dataref->MSFSvar, dataref->unit, type);
    if (FAILED(hr)) {
        qCritical() << "Failed to add to data definition : " << dataref->MSFSvar;
    }
    if (!timers.contains(frequency)) timers[frequency].start();
    
    SimConnect_RequestDataOnSimObject(hSimConnect, dataref->requestId, definition, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_SIM_FRAME, SIMCONNECT_DATA_REQUEST_FLAG_CHANGED);
}

/**
 * @brief Ajoute une variable de simulation à la liste des variables à lire a partir d'un dataref et de ça fréquence.
 * 
 * @param var Le dataref de la variable demandée
 * @param frequency La fréquence d'émission des réponses ( vers FlightSim ).
 * @param id L'identifiant unique de la requête.
 * @return void
 */
void LLFTranslator::addVariable(const QString &var, int frequency, int id)
{
    string MSFSvarStr = translateXPlaneToMFS(var).toStdString();
    char* MSFSvar = new char[MSFSvarStr.length() + 1];
    strcpy(MSFSvar, MSFSvarStr.c_str());

    string unitStr = getXPlaneUnit(var).toStdString();
    char* unit = new char[unitStr.length() + 1];
    strcpy(unit, unitStr.c_str());
    
    Dataref* dataref = new Dataref(var, frequency);
    dataref->MSFSvar = MSFSvar;
    dataref->unit = unit;
    dataref->requestId = id;

    if (MSFSvarStr == "Not Found" || unitStr == "Not Found") return;
    qDebug() << "Adding variable " << var << "( " << translateXPlaneToMFS(var) << " ) " << " with frequency " << frequency << "Hz";
    
    w->Data->getParent()->listWidget->addItem(MSFSvar);
    w->Data->addPointer(dataref);

    readVar(dataref, SIMCONNECT_DATATYPE_FLOAT64, [this,dataref](double value) {
        dataref->value = applyEquation(dataref->name, value);
    }, frequency);
}

/**
 * @brief Supprime une variable de la liste des variables à lire.
 * 
 * @param id Idifiant unique de la requête.
 * @return void
 * 
 * La méthode est appelée lorsque la fréquence d'un variable demandé est inférieure ou égale à 0.
 */
void LLFTranslator::removeVariable(int id)
{
    Dataref* dataref = variables.value(id);
    if (dataref == nullptr) return;

    qDebug() << "Removing variable " << dataref->name << " with id " << id;

    w->Data->removeFromList(dataref->MSFSvar);
    w->Data->removePointer(dataref);
    w->Data->initCollumn();
    variables.remove(id);
}

/**
 * @brief Fonction de rappel pour traité les données reçues de SimConnect.
 * 
 * @param pData Les données reçues.
 * @param cbData La taille des données reçues.
 * @param pContext Le contexte de la fonction de rappel.
 * @return void
 * 
 * Cette fonction est appelée à chaque fois que des données sont reçues de SimConnect.
 * Quand elle reçoit une variables de simulation, elle appelle la fonction de rappel de la variable.
 */
void CALLBACK DispatchProcRD(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext) {
    QHash<DWORD, Dataref*>* variables = static_cast<QHash<DWORD, Dataref*>*>(pContext);
    SIMCONNECT_RECV_EVENT *evt;
    SIMCONNECT_RECV_SIMOBJECT_DATA *pObjData;
    SIMCONNECT_RECV_EXCEPTION *except;
    DWORD request;

    switch(pData->dwID) {
        case SIMCONNECT_RECV_ID_OPEN:
            //qDebug() << "SimConnect connection opened";
            break;

        case SIMCONNECT_RECV_ID_EVENT:
            evt = (SIMCONNECT_RECV_EVENT*) pData;
            qDebug() << "Received event with ID " << evt->uEventID;
            break;

        case SIMCONNECT_RECV_ID_SIMOBJECT_DATA:
            pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA*) pData;
            request = pObjData->dwRequestID;
            if (!variables->contains(request) || variables->value(request) == nullptr) break;
            variables->value(request)->callback(pObjData);
            break;

        case SIMCONNECT_RECV_ID_EXCEPTION:
            except = (SIMCONNECT_RECV_EXCEPTION*) pData;
            qWarning() << "SimConnect exception: " << except->dwException;
            break;

        case SIMCONNECT_RECV_ID_QUIT:
            qDebug() << "SimConnect connection closed";
            break;

        default:
            break;
    }
}

/**
 * @brief Initialise le lecteur de variable de simulation.
 * 
 * Je suis quasiment sûr que cette fonction est inutile, je pense que l'utilisation thread est surperflu
 * Un seul appel devrais suffire ( sauf pour iniData ducoup )
 * A optimiser
 * 
 * @return void
 */
void LLFTranslator::initSimReader()
{
    QThread* thread = QThread::create([this] {
        while (connected) {
            SimConnect_CallDispatch(hSimConnect, DispatchProcRD, &variables);
            w->Data->initData(variables.values());
            QThread::msleep(1000/maxFrequency);
        }
    });

    QObject::connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    thread->start();
}

/**
 * @brief Initialise le travailleur UDP.
 * 
 * @return void
 * 
 * Cette fonction crée un thread qui envoie les données des variables de simulation.
 * Les données sont envoyées à la fréquence demandée en se basant sur un timer.
 * Le timer et remis à zéro à chaque fois que la fréquence est atteinte.
 * Ensuite elle ajotue la variable à une QList afin de les envoyer simultanément a l'aide des méthodes de l'UdpWorker.
 */
void LLFTranslator::initUdpWorker()
{
    QThread* thread = QThread::create([this] {
        QList<QPair<QString, float>> datagrams;
        while (connected) {
            datagrams.clear();
            for (auto it = timers.begin(); it != timers.end(); it++) {
                if (it.value().elapsed() < 1000 / it.key()) continue;
                it.value().restart();

                for (Dataref* variable : variables.values()) {
                    if (variable->frequency == it.key()) {
                        datagrams.append(QPair<QString, float>(variable->name, variable->value));
                    }
                }
            }
            udpWorker->sendFrame(datagrams);

            QThread::msleep(1000/maxFrequency);
        }
    });

    QObject::connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    QObject::connect(udpWorker, udpWorker->datagramReceived, this, onDatagramReceived);
    thread->start();
    udpWorker->init();
}

/**
 * @brief Connection a SimConnect.
 * 
 * @return void
 */
bool LLFTranslator::connect() {
    if (SUCCEEDED(SimConnect_Open(&hSimConnect, "LLFTranslator", NULL, 0, 0, 0)))
    {
        connected = true;
        qInfo() << "Connected to MFS!";
        initSimReader();
        initUdpWorker();
        return true;
    }
    else
    {
        connected = false;
        qWarning() << "Failed to connect to MFS!";
        return false;
    }
}

/**
 * @brief Déconnecte de SimConnect.
 * 
 * @return void
 * 
 * Il y a clairement un problème avec les méthodes de connection et déconnection 
 * Si on utilise le bouton de l'IHM afin de ce connecter et déconnecter plusieurs fois, on reçoit les variables plusieurs fois
 * A fix 
 */
bool LLFTranslator::disconnect() {
    if (isConnected()) {
        if (SUCCEEDED(SimConnect_Close(hSimConnect))) {
            connected = false;
            qInfo() << "Disconnected from MFS!";
            return true;
        }
        else {
            qWarning() << "Failed to disconnect from MFS!";
            return false;
        }
    }
    return true;
}

/**
 * @brief Charge la configuration.
 * 
 * @return QList<QString> Le contenu de chaque ligne du fichier de configuration.
 */
QList<QString> LLFTranslator::loadConfig()
{
    QList<QString> list;
    QFile file(":src/config.csv");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "Impossible d'ouvrir le fichier : " << file.errorString();
        return list;
    }

    QTextStream in(&file);
    QString fileContent;
    while (!in.atEnd()) {
        QString line = in.readLine();
        fileContent += line + ',';
    }

    list = fileContent.split(",");
    config = list;

    file.close();
    return list;
}

/**
 * @brief Lit un argument d'une ligne du fichier de configuration.
 * 
 * @param dataref Le dataref dont on veut lire la configuration.
 * @param arg L'index de l'argument à lire.
 * @return QString L'argument lu.
 */
QString LLFTranslator::readCsvArg(QString dataref,int arg)
{
    for(int i = 0 ;i < (int)config.size()-1 ;i++){
        if (config[i] == dataref)
        {
            return config[i + arg];
        }
    }
    return "Not Found";
}

/**
 * @brief Fonction de rappel pour traité les données reçues de l'UdpWorker.
 * 
 * @param dataref Le dataref de la variable de simulation.
 * @param frequency La fréquence d'émission des réponses ( vers FlightSim ).
 * @param id L'identifiant unique de la requête.
 * @return void
 * 
 * Cette fonction est appelée à chaque fois que des données sont reçues de l'UdpWorker.
 * Elle ajoute ou supprime une variable de la liste des variables à lire.
 */
void LLFTranslator::onDatagramReceived(char* dataref, int frequency, int id)
{
    if (!isConnected()) return;
    if ( frequency <= 0 ) removeVariable(id);
    else addVariable(QString(dataref), frequency, id);
}

/**
 * @brief Traduit un dataref XPlane en dataref MFS.
 * 
 * @param dataref Le dataref XPlane à traduire.
 * @return QString Le dataref MFS traduit.
 * 
 * Ne prend pas en compte les priorités des opérateurs.
 */
double LLFTranslator::applyEquation(const QString& dataref,double value)
{
    QString equation = getEquation(dataref);
    equation.replace("value", QString::number(value));
    
    std::stringstream ss(equation.toStdString());
    double result = 0.0;
    double number = 0.0;
    char operation = '+';

    while (ss >> number) { // Dans le fichier de configuration, si on souhaite une variable négatif, il faut faire value*-1 et non -value.
        switch (operation) {
            case '+': result += number; break;
            case '-': result -= number; break;
            case '*': result *= number; break;
            case '/': if (number != 0.0) result /= number; break;
            case '%': result = fmod(result,number); break;
            default: /* Handle error */ break;
        }

        ss >> operation;
    }

    return result;
}

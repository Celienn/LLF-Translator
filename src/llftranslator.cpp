#include "llftranslator.h"

struct Struct1
{
    double altitude;
};

LLFTranslator::LLFTranslator()
{
    config = loadConfig();
    udpWorker = new UDPWorker(this);
}

LLFTranslator::~LLFTranslator()
{

}

void LLFTranslator::addVariable(const QString &var, int frequency)
{
    string MFSvarStr = translateXPlaneToMFS(var).toStdString();
    char* MFSvar = new char[MFSvarStr.length() + 1];
    strcpy(MFSvar, MFSvarStr.c_str());

    string unitStr = getXPlaneUnit(var).toStdString();
    char* unit = new char[unitStr.length() + 1];
    strcpy(unit, unitStr.c_str());
    
    Dataref* dataref = new Dataref(var, frequency, 0.0);
    variables.append(dataref);

    if (MFSvarStr == "Not Found" || unitStr == "Not Found") return;
    qDebug() << "Adding variable " << var << "( " << translateXPlaneToMFS(var) << " ) " << " with frequency " << frequency << "Hz";
    
    readVar<double>(MFSvar, unit, SIMCONNECT_DATATYPE_FLOAT64, [this,dataref](double value) {
        dataref->value = applyEquation(dataref->name, value);
    }, frequency);
}

// Merci Copilot :D
void CALLBACK DispatchProcRD(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext) {
    LLFTranslator* translator = static_cast<LLFTranslator*>(pContext);
    SIMCONNECT_RECV_EVENT *evt;
    SIMCONNECT_RECV_SIMOBJECT_DATA *pObjData;
    SIMCONNECT_RECV_EXCEPTION *except;
    DWORD request;

    switch(pData->dwID) {
        case SIMCONNECT_RECV_ID_OPEN:
            qDebug() << "SimConnect connection opened";
            break;

        case SIMCONNECT_RECV_ID_EVENT:
            evt = (SIMCONNECT_RECV_EVENT*) pData;
            qDebug() << "Received event with ID " << evt->uEventID;
            break;

        case SIMCONNECT_RECV_ID_SIMOBJECT_DATA:
            pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA*) pData;
            request = pObjData->dwRequestID;
            if (translator->callbacks.count(request)) {
                translator->callbacks[request](pObjData);
            }
            break;

        case SIMCONNECT_RECV_ID_EXCEPTION:
            except = (SIMCONNECT_RECV_EXCEPTION*) pData;
            qDebug() << "SimConnect exception: " << except->dwException;
            break;

        case SIMCONNECT_RECV_ID_QUIT:
            qDebug() << "SimConnect connection closed";
            break;

        default:
            break;
    }
}

void LLFTranslator::initUdpWorker()
{
    QThread* thread = QThread::create([this] {
        QList<QPair<QString, float>> datagrams;
        while (true) {
            datagrams.clear();
            for (auto it = timers.begin(); it != timers.end(); it++) {
                if (it.value().elapsed() >= 1000 / it.key()) {
                    it.value().restart();

                    for (Dataref* variable : variables) {
                        if (variable->frequency == it.key() && variable->value != 0.0) {
                            datagrams.append(QPair<QString, float>(variable->name, variable->value));
                        }
                    }
                }
            }
            udpWorker->sendFrame(datagrams);
            // Sleep for 50ms so it executes at 20Hz 
            QThread::msleep(1000/20);
        }
    });

    QObject::connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    QObject::connect(udpWorker, udpWorker->datagramReceived, this, onDatagramReceived);
    thread->start();
    udpWorker->init();
}

void LLFTranslator::connect() {
    if (SUCCEEDED(SimConnect_Open(&hSimConnect, "LLFTranslator", NULL, 0, 0, 0)))
    {
        connected = true;
        qDebug() << "Connected to MFS!";
        initUdpWorker();
    }
    else
    {
        connected = false;
        qDebug() << "Failed to connect to MFS!";
    }
}

QList<QString> LLFTranslator::loadConfig()
{
    QList<QString> list;
    QFile file(":src/config.csv");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Impossible d'ouvrir le fichier : " << file.errorString();
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

void LLFTranslator::onDatagramReceived(char* dataref, int frequency)
{
    addVariable(QString(dataref), frequency);
}

// Ne prend pas en compte les priorités des opération
double LLFTranslator::applyEquation(const QString& dataref,double value)
{
    QString equation = getEquation(dataref);
    equation.replace("value", QString::number(value));
    
    std::stringstream ss(equation.toStdString());
    double result = 0.0;
    double number = 0.0;
    char operation = '+';

    while (ss >> number) {
        switch (operation) {
            case '+': result += number; break;
            case '-': result -= number; break;
            case '*': result *= number; break;
            case '/': if (number != 0.0) result /= number; break;
            case '%': result = static_cast<int>(result) % static_cast<int>(number); break;
            default: /* Handle error */ break;
        }

        ss >> operation;
    }

    return result;
}

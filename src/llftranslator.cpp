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
    Dataref* dataref = new Dataref(var, frequency, 0.0);
    variables.append(dataref);
    qDebug() << "Adding variable " << var << " with frequency " << frequency << "Hz";
    readVar<double>(translateXPlaneToMFS(var), getXPlaneUnit(var), SIMCONNECT_DATATYPE_FLOAT64, [this,dataref](double value) {
        dataref->value = value;
        qDebug() << "Received value " << value << " for " << dataref->name;
    }, frequency);
}

// Merci Copilot :D
void CALLBACK DispatchProcRD(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext) {
    LLFTranslator* translator = static_cast<LLFTranslator*>(pContext);
    SIMCONNECT_RECV_SIMOBJECT_DATA *pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA*)pData;
    DWORD request = pObjData->dwRequestID;
    qDebug() << "Received data for request " << request;
    if (translator->callbacks.count(request)) {
        translator->callbacks[request](pObjData);
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
                        if (variable->frequency == it.key()) {
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
    HRESULT hr;
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

char* LLFTranslator::translateXPlaneToMFS(QString ref)
{
    for(int i = 0 ;i < (int)config.size()-1 ;i++){
        if (config[i] == ref)
        {
            return (char*)(config[i + 1].toStdString()).c_str();
        }
    }
    return "Not Found";
}

char* LLFTranslator::getXPlaneUnit(QString ref)
{
    for(int i = 0 ;i < (int)config.size()-1 ;i++){
        if (config[i] == ref)
        {
            return (char*)(config[i + 2].toStdString()).c_str();
        }
    }
    return "Not Found";
}

void LLFTranslator::onDatagramReceived(char* rref, int frequency)
{
    addVariable(QString(rref), frequency);
}

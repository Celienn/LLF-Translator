#include "llftranslator.h"

struct Struct1
{
    double altitude;
};

LLFTranslator::LLFTranslator()
{
    loadConfig();
    udpWorker = new UDPWorker(this);
}

LLFTranslator::~LLFTranslator()
{

}

// TODO : Use QString instead of string
void LLFTranslator::addVariable(const string &var, int frequency)
{
    Dataref* dataref = new Dataref(QString::fromStdString(var), frequency, 0.0);
    variables.append(dataref);
    qDebug() << "Adding variable " << QString::fromStdString(var) << " with frequency " << frequency << "Hz";
    readVar<double>(translateXPlaneToMFS(var.c_str()), getXPlaneUnit(var.c_str()), SIMCONNECT_DATATYPE_FLOAT64, [this,dataref](double value) {
        dataref->value = value;
    }, frequency);
}

void LLFTranslator::addVariable(const vector<string> var, int frequency){
    for (int i = 0; i < var.size(); i++){
        addVariable(var[i], frequency); 
    }
};

// Merci Copilot :D
void CALLBACK DispatchProcRD(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext) {
    LLFTranslator* translator = static_cast<LLFTranslator*>(pContext);
    SIMCONNECT_RECV_SIMOBJECT_DATA *pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA*)pData;
    DWORD request = pObjData->dwRequestID;
    if (translator->callbacks.count(request)) {
        translator->callbacks[request](pObjData);
    }
}

void LLFTranslator::connect() {
    HRESULT hr;
    if (SUCCEEDED(SimConnect_Open(&hSimConnect, "LLFTranslator", NULL, 0, 0, 0)))
    {
        connected = true;
        qDebug() << "Connected to MFS!";
        QObject::connect(udpWorker, udpWorker->datagramReceived, this, onDatagramReceived);
        udpWorker->init();
    }
    else
    {
        connected = false;
        qDebug() << "Failed to connect to MFS!";
    }
}

vector<string> split(string str, string delimiter){
    size_t pos = 0;
    string token;
    vector<string> tokens;

    while ((pos = str.find(delimiter)) != string::npos) {
        token = str.substr(0, pos);
        tokens.push_back(token);
        str.erase(0, pos + delimiter.length());
    }

    tokens.push_back(str);
    return tokens;
}

vector<string> LLFTranslator::loadConfig()
{
    vector<string> table;
    QFile file(":src/config.csv");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Impossible d'ouvrir le fichier : " << file.errorString();
        return table;
    }

    QTextStream in(&file);
    string fileContent;
    while (!in.atEnd()) {
        QString line = in.readLine();
        fileContent += line.toStdString() + ',';
    }

    table = ::split(fileContent, ",");
    config = table;
    file.close();
    return table;
}

const char* LLFTranslator::translateXPlaneToMFS(string ref)
{
    for(int i = 0 ;i < (int)config.size()-1 ;i++){
        if (config[i] == ref)
        {
            return config[i + 1].c_str();
        }
    }
    return "Not Found";
}

const char* LLFTranslator::getXPlaneUnit(string ref)
{
    for(int i = 0 ;i < (int)config.size()-1 ;i++){
        if (config[i] == ref)
        {
            return config[i + 2].c_str();
        }
    }
    return "Not Found";
}

void LLFTranslator::onDatagramReceived(char* rref, int frequency)
{
    addVariable(rref, frequency);
}
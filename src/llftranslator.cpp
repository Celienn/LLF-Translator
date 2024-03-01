#include "llftranslator.h"

struct Struct1
{
    double altitude;
};

enum DATA_DEFINE_ID
{
    DEFINITION_1,
};

enum DATA_REQUEST_ID
{
    REQUEST_1,
};


LLFTranslator::LLFTranslator()
{

}

LLFTranslator::~LLFTranslator()
{

}

void LLFTranslator::addVariable(const string &var )
{
    variables.push_back(var);
}

void LLFTranslator::addVariable( vector<string> var){
    for (int i = 0; i < var.size(); i++){
        variables.push_back(var[i]);
    }
};

// Merci Copilot :D
void CALLBACK DispatchProcRD(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext) {
    LLFTranslator* translator = static_cast<LLFTranslator*>(pContext);
    SIMCONNECT_RECV_SIMOBJECT_DATA *pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA*)pData;
    DWORD request = pObjData->dwRequestID;
    if (translator->callbacks.count(request)) {
        translator->callbacks[request]();
    }
}

void LLFTranslator::readVar(const char * MFSvar, const char * unit, SIMCONNECT_DATATYPE type, function<void()> callback, int frequency){
    hash<string> hasher;
    DWORD definition = hasher(MFSvar) % 1000;
    DWORD request = hasher(string(MFSvar) + string(unit))  % 1000;
    callbacks[request] = callback;
    thread Thread([this,MFSvar,unit,type,frequency,definition,request]() {
        if (!isConnected()) return; 
        SimConnect_AddToDataDefinition(hSimConnect, definition, MFSvar, unit, type);
        while(true) 
        {
            // Je met 0 pour le moment afin de tester
            SimConnect_RequestDataOnSimObject(hSimConnect, request, definition, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_ONCE);
            SimConnect_CallDispatch(hSimConnect, DispatchProcRD, this);
            Sleep(1000/frequency);
        }
    });

    Thread.detach();
}



void LLFTranslator::connect() {
    HRESULT hr;
    if (SUCCEEDED(SimConnect_Open(&hSimConnect, "LLFTranslator", NULL, 0, 0, 0)))
    {
        connected = true;
        qDebug() << "Connected to MFS!";

    }
    else
    {
        connected = false;
        qDebug() << "Failed to connect to MFS!";
    }
}

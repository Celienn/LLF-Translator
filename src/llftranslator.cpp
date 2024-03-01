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

void CALLBACK MyDispatchProcRD(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext)
{
    switch(pData->dwID)
    {
        case SIMCONNECT_RECV_ID_SIMOBJECT_DATA:
        {
            SIMCONNECT_RECV_SIMOBJECT_DATA *pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA*)pData;

            switch(pObjData->dwRequestID)
            {
                case REQUEST_1:
                {
                    Struct1 *pS = (Struct1*)&pObjData->dwData;
                    qDebug() << pS->altitude;
                    break;
                }

                default:
                    break;
            }
            break;
        }

        default:
            break;
    }
}

// USAGE :
// readValue("Plane Altitude", "feet", SIMCONNECT_DATATYPE_FLOAT64,callback);
void LLFTranslator::readVar(const char * MFSvar, const char * unit, SIMCONNECT_DATATYPE type,int frequency){
    thread Thread([this,MFSvar,unit,type,frequency]() {
        if (!isConnected()) return; 
        SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, MFSvar, unit, type);
        while(true) 
        {
            SimConnect_RequestDataOnSimObject(hSimConnect, REQUEST_1, DEFINITION_1, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_ONCE);
            SimConnect_CallDispatch(hSimConnect, MyDispatchProcRD, NULL);
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

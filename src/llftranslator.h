#ifndef LLFTRANSLATOR_H
#define LLFTRANSLATOR_H

#include <QWidget>
#include <string>
#include <vector>
#include <thread>
#include <windows.h>
#include <SimConnect.h>

using namespace std;

void CALLBACK DispatchProcRD(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext);

class LLFTranslator
{

    public:
        LLFTranslator();
        ~LLFTranslator();
        void addVariable(const string &var);
        void addVariable(const vector<string> var);
        bool isConnected() { return connected; };
        void connect();
        void readVar(const char * MFSvar, const char * unit, SIMCONNECT_DATATYPE type,function<void()> callback, int frequency = 1);
        friend void CALLBACK DispatchProcRD(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext);
    
        vector<string> loadConfig();
        string translateXPlaneToMFS(string ref);
    private:
        vector<string> config;
        bool connected;
        HANDLE hSimConnect;
        HRESULT hr;
        map<DWORD, function<void()>> callbacks;
};
#endif // LLFTRANSLATOR_H

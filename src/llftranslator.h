#ifndef LLFTRANSLATOR_H
#define LLFTRANSLATOR_H

#include <QWidget>
#include <string>
#include <vector>
#include <thread>
#include <windows.h>
#include <SimConnect.h>

using namespace std;

class LLFTranslator
{

    public:
        LLFTranslator();
        ~LLFTranslator();
        void addVariable(const string &var);
        void addVariable(const vector<string> var);
        bool isConnected() { return connected; };
        void connect();
        void readVar(const char * MFSvar, const char * unit, SIMCONNECT_DATATYPE type,int frequency = 1);
        
    private:
        vector<string> variables;
        bool connected;
        HANDLE hSimConnect;
        HRESULT hr;
};
#endif // LLFTRANSLATOR_H

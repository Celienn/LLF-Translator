#ifndef LLFTRANSLATOR_H
#define LLFTRANSLATOR_H

#include <QWidget>
#include <string>
#include <vector>
#include <QThread>
#include <windows.h>
#include <SimConnect.h>
#include <QFile>
#include <udpworker.h>

using namespace std;

void CALLBACK DispatchProcRD(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext);

class LLFTranslator : public QObject
{

    public:
        LLFTranslator();
        ~LLFTranslator();
        void addVariable(const string &var, int frequency = 0);
        void addVariable(const vector<string> var, int frequency = 0);
        bool isConnected() { return connected; };
        void connect();
        friend void CALLBACK DispatchProcRD(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext);

        template <typename T>
        void readVar(const char * MFSvar, const char * unit, SIMCONNECT_DATATYPE type, function<void(T)> callback, int frequency){
            hash<string> hasher;
            DWORD definition = hasher(MFSvar) % 1000;
            DWORD request = hasher(string(MFSvar) + string(unit))  % 1000;
            callbacks[request] = [callback](SIMCONNECT_RECV_SIMOBJECT_DATA *pObjData){
                T* data = (T*)&pObjData->dwData;
                callback(*data);
            };
            QThread* thread = QThread::create([this, MFSvar, unit, type, frequency, definition, request] {
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

            QObject::connect(thread, &QThread::finished, thread, &QThread::deleteLater);
            thread->start();
        }
        
        // Temporaire pour le debug
        const char* translateXPlaneToMFS(string ref);
    private:
        vector<string> config;
        vector<string> variables;
        bool connected;
        HANDLE hSimConnect;
        HRESULT hr;
        map<DWORD, function<void(SIMCONNECT_RECV_SIMOBJECT_DATA*)>> callbacks;
        UDPWorker *udpWorker;
        
        friend void CALLBACK DispatchProcRD(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext);
        vector<string> loadConfig();
    public slots:
        void onDatagramReceived(char* rref, int frequency);
};

#endif // LLFTRANSLATOR_H

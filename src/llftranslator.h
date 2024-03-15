#ifndef LLFTRANSLATOR_H
#define LLFTRANSLATOR_H

#include <QWidget>
#include <string>
#include <vector>
#include <QVector3D>
#include <QThread>
#include <windows.h>
#include <SimConnect.h>
#include <QFile>
#include <udpworker.h>
#include <dataref.h>

using namespace std;

void CALLBACK DispatchProcRD(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext);

// TODO : Use QString instead of string
// TODO : Use QHash instead of map
// TODO : Check that all include are necessary
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
            DWORD definition = hasher(MFSvar) % 10000;
            DWORD request = hasher(string(MFSvar) + string(unit))  % 10000;
            callbacks[request] = [callback, this, request, frequency](SIMCONNECT_RECV_SIMOBJECT_DATA *pObjData){
                T* data = (T*)&pObjData->dwData;

                if (!timers.contains(request)) timers[request].start();
                
                if (timers[request].elapsed() >= 1000 / frequency) {
                    callback(*data);
                    timers[request].restart();
                }
            };
            QThread* thread = QThread::create([this, MFSvar, unit, type, frequency, definition, request] {
                if (!isConnected()) return; 
                SimConnect_AddToDataDefinition(hSimConnect, definition, MFSvar, unit, type);
                SimConnect_RequestDataOnSimObject(hSimConnect, request, definition, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_SIM_FRAME);
                
                while(true) 
                {
                    SimConnect_CallDispatch(hSimConnect, DispatchProcRD, this);
                    QThread::msleep(1000);
                }
            });

            QObject::connect(thread, &QThread::finished, thread, &QThread::deleteLater);
            thread->start();
        }
        
        // Temporaire pour le debug
        const char* translateXPlaneToMFS(string ref);
        const char* getXPlaneUnit(string ref);
    private:
        vector<string> config;
        QList<Dataref*> variables;
        bool connected;
        HANDLE hSimConnect;
        HRESULT hr;
        map<DWORD, function<void(SIMCONNECT_RECV_SIMOBJECT_DATA*)>> callbacks;
        QHash<int, QElapsedTimer> timers;
        UDPWorker *udpWorker;
        
        friend void CALLBACK DispatchProcRD(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext);
        vector<string> loadConfig();
    public slots:
        void onDatagramReceived(char* rref, int frequency);
};

#endif // LLFTRANSLATOR_H
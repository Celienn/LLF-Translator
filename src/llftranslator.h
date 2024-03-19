#ifndef LLFTRANSLATOR_H
#define LLFTRANSLATOR_H

#include <string>
#include <vector>
#include <QVector3D>
#include <QThread>
#include <QWidget>
#include <QFile>
#include <windows.h>
#include <SimConnect.h>
#include <udpworker.h>
#include <QElapsedTimer>
#include <dataref.h>
#include <QVector>

using namespace std;

void CALLBACK DispatchProcRD(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext);

// TODO : Use QString instead of string
// TODO : Use QHash instead of map
// TODO : Check that all include are necessary
// TODO : Use QElapsedTimer instead of QTime
// TODO : Use QHast instead of map

class LLFTranslator : public QObject
{

    public:
        LLFTranslator();
        ~LLFTranslator();
        void addVariable(const QString &var, int frequency = 0);
        bool isConnected() { return connected; };
        void connect();
        friend void CALLBACK DispatchProcRD(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext);

        // TODO : use QHash and QString
        template <typename T>
        void readVar(const char * MFSvar, const char * unit, SIMCONNECT_DATATYPE type, function<void(T)> callback, int frequency){
            hash<string> hasher;
            DWORD definition = hasher(MFSvar) % 10000;
            DWORD request = hasher(string(MFSvar) + string(unit))  % 10000;
            callbacks[request] = [callback, this, request, frequency](SIMCONNECT_RECV_SIMOBJECT_DATA *pObjData){
                T* data = (T*)&pObjData->dwData;
                callback(*data);
            };
            QThread* thread = QThread::create([this, MFSvar, unit, type, frequency, definition, request] {
                if (!isConnected()) return; 
                SimConnect_AddToDataDefinition(hSimConnect, definition, MFSvar, unit, type);
                qDebug() << request;
                SimConnect_RequestDataOnSimObject(hSimConnect, request, definition, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_SIM_FRAME);
                
                if (!timers.contains(frequency)) timers[frequency].start();
                
                while(true) 
                {
                    SimConnect_CallDispatch(hSimConnect, DispatchProcRD, this);
                    QThread::msleep(1000 / frequency);
                }
            });

            QObject::connect(thread, &QThread::finished, thread, &QThread::deleteLater);
            thread->start();
        }
        
    private:
        QList<QString> config;
        QList<Dataref*> variables;
        bool connected;
        HANDLE hSimConnect;
        HRESULT hr;
        map<DWORD, function<void(SIMCONNECT_RECV_SIMOBJECT_DATA*)>> callbacks;
        QHash<int, QElapsedTimer> timers;
        UDPWorker *udpWorker;
        
        friend void CALLBACK DispatchProcRD(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext);
        char* translateXPlaneToMFS(QString ref);
        char* getXPlaneUnit(QString ref);
        QList<QString> loadConfig();
        void initUdpWorker();
    public slots:
        void onDatagramReceived(char* rref, int frequency);
};

#endif // LLFTRANSLATOR_H
#ifndef LLFTRANSLATOR_H
#define LLFTRANSLATOR_H

#include <QThread>
#include <QFile>
#include <windows.h>
#include <SimConnect.h>
#include <udpworker.h>
#include <QElapsedTimer>
#include <dataref.h>
#include <QVector>
#include <cmath>
#include "mainwindow.h"

using namespace std;

void CALLBACK DispatchProcRD(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext);

class LLFTranslator : public QObject
{

    public:
        LLFTranslator();
        ~LLFTranslator();
        void addVariable(const QString &var, int frequency = 0);
        bool isConnected() { return connected; };
        void connect();
        friend void CALLBACK DispatchProcRD(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext);

        template <typename T>
        void readVar(const char * MFSvar, const char * unit, SIMCONNECT_DATATYPE type, function<void(T)> callback, int frequency){
            if (!isConnected()) return;
            DWORD definition = nextDefinitionId++;
            DWORD request = nextRequestId++;
            callbacks[request] = [callback, this, request, frequency](SIMCONNECT_RECV_SIMOBJECT_DATA *pObjData){
                T* data = (T*)&pObjData->dwData;
                callback(*data);
            };
            
            HRESULT hr = SimConnect_AddToDataDefinition(hSimConnect, definition, MFSvar, unit, type);
            if (FAILED(hr)) {
                qDebug() << "Failed to add to data definition : " << MFSvar;
            }
            if (!timers.contains(frequency)) timers[frequency].start();
            
            SimConnect_RequestDataOnSimObject(hSimConnect, request, definition, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_SIM_FRAME, SIMCONNECT_DATA_REQUEST_FLAG_CHANGED);
        }
        
    private:
        QList<QString> config;
        QList<Dataref*> variables;
        bool connected;
        HANDLE hSimConnect;
        DWORD nextDefinitionId = 0;
        DWORD nextRequestId = 0;
        int maxFrequency = 20;
        QHash<DWORD, function<void(SIMCONNECT_RECV_SIMOBJECT_DATA*)>> callbacks;
        QHash<int, QElapsedTimer> timers;
        UDPWorker *udpWorker;
        MainWindow *w;
        
        friend void CALLBACK DispatchProcRD(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext);
        QString readCsvArg(QString dataref,int arg);
        QString translateXPlaneToMFS(QString dataref)   { return readCsvArg(dataref, 1); };
        QString getXPlaneUnit(QString dataref)          { return readCsvArg(dataref, 2); };
        QString getEquation(QString dataref)            { return readCsvArg(dataref, 3); };
        double applyEquation(const QString& dataref,double value);
        QList<QString> loadConfig();
        void removeVariable(const QString &var, int id);
        void initUdpWorker();
        void initSimReader();
        
    public slots:
        void onDatagramReceived(char* dataref, int frequency, int id);
};

#endif // LLFTRANSLATOR_H

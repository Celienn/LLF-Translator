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
#include "exprtk.hpp"

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
            DWORD definition = nextDefinitionId++;
            DWORD request = nextRequestId++;
            callbacks[request] = [callback, this, request, frequency](SIMCONNECT_RECV_SIMOBJECT_DATA *pObjData){
                T* data = (T*)&pObjData->dwData;
                callback(*data);
            };
            QThread* thread = QThread::create([this, MFSvar, unit, type, frequency, definition, request] {
                if (!isConnected()) return; 
                qDebug() << MFSvar;
                qDebug() << unit;
                HRESULT hr = SimConnect_AddToDataDefinition(hSimConnect, definition, MFSvar, unit, type);
                if (FAILED(hr)) {
                    // Gérer l'erreur
                    qDebug() << "Failed to add to data definition";
                }
                if (!timers.contains(frequency)) timers[frequency].start();
                
                while(true) 
                {
                    SimConnect_RequestDataOnSimObject(hSimConnect, request, definition, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_SIM_FRAME, SIMCONNECT_DATA_REQUEST_FLAG_CHANGED);
                    
                    SimConnect_CallDispatch(hSimConnect, DispatchProcRD, this);
                    QThread::msleep(1000);
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
        DWORD nextDefinitionId = 0;
        DWORD nextRequestId = 0;
        map<DWORD, function<void(SIMCONNECT_RECV_SIMOBJECT_DATA*)>> callbacks;
        QHash<int, QElapsedTimer> timers;
        QHash<QString, function<double(double)>> equations;
        UDPWorker *udpWorker;
        
        friend void CALLBACK DispatchProcRD(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext);
        QString readCsvArg(QString dataref,int arg);
        QString translateXPlaneToMFS(QString dataref)   { return readCsvArg(dataref, 1); };
        QString getXPlaneUnit(QString dataref)          { return readCsvArg(dataref, 2); };
        QString getEquation(QString dataref)            { return readCsvArg(dataref, 3); };
        double applyEquation(const QString& dataref,double value);
        QList<QString> loadConfig();
        void initUdpWorker();
    public slots:
        void onDatagramReceived(char* dataref, int frequency);
};

#endif // LLFTRANSLATOR_H
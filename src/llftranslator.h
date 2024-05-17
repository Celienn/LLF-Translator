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
#include "globaldata.h"

#define maxFrequency 20

using namespace std;

class MainWindow;

void CALLBACK DispatchProcRD(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext);

// TODO : Fix when connecting and disconnecting multiple times, variables are received multiple times

class LLFTranslator : public QObject
{

    public:
        LLFTranslator();
        ~LLFTranslator();
        void addVariable(const QString &var, int frequency, int id);
        void removeVariable(int id);
        bool isConnected() { return connected; };
        bool connect();
        bool disconnect();
        friend void CALLBACK DispatchProcRD(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext);
        void readVar(Dataref* dataref, SIMCONNECT_DATATYPE type, function<void(double)> callback, int frequency);
        
    private:
        QList<QString> config;
        QHash<DWORD, Dataref*> variables;
        bool connected;
        HANDLE hSimConnect;
        DWORD nextDefinitionId = 0;
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
        void initUdpWorker();
        void initSimReader();
        
    public slots:
        void onDatagramReceived(char* dataref, int frequency, int id);
};

#endif // LLFTRANSLATOR_H

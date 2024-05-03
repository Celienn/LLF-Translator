#ifndef DATAREF_H
#define DATAREF_H

#include <QObject>
#include "Windows.h"
#include <SimConnect.h>

using namespace std;

class Dataref : public QObject
{
    public:
        Dataref(QString name, int frequency, double value = 0.0);
        ~Dataref();
        QString name;
        const char * MSFSvar;
        const char * unit;
        int frequency;
        double value;
        DWORD definitionId;
        DWORD requestId;
        function<void(SIMCONNECT_RECV_SIMOBJECT_DATA*)> callback;
}   ;

#endif // LLFTRANSLATOR_H
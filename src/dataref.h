#ifndef DATAREF_H
#define DATAREF_H

#include <QObject>

class Dataref : public QObject
{
    public:
        Dataref(QString name, int frequency, double value);
        ~Dataref();
        QString name;
        int frequency;
        double value;
}   ;

#endif // LLFTRANSLATOR_H
#ifndef GLOBALDATA_H
#define GLOBALDATA_H

#include <unordered_map>
#include <QtCore/QCoreApplication>
#include <string>
#include <QHash>
#include <QHashIterator>
#include <QString>
class GlobalData {
public:
    GlobalData();
    void trackVariable(QString key, double* value);
    void unTrackVariable(QString key);
    double getValue(QString key);
    QHash<QString, double*> getHash(){return hash ;}

private:
    QHash<QString, double*> hash;
};

#endif // GLOBALDATA_H

#ifndef GLOBALDATA_H
#define GLOBALDATA_H

#include <unordered_map>
#include <QtCore/QCoreApplication>
#include <string>
#include <QHash>
#include <QHashIterator>
#include <QString>
#include <mainwindow.h>
#include <QList>
#include "ui_mainwindow.h"
#include "dataref.h"

class GlobalData {

    public:
        GlobalData();
        void trackVariable(QString key, double* value);
        void unTrackVariable(QString key);
        void initCollumn(Ui::MainWindow *ui);
        void initData(Ui::MainWindow *ui);
        void initTable();
        void setcase(Ui::MainWindow *ui,int i, int row, QString text);
        double getValue(QString key);
        QList<Dataref> getDataRefs(){return DListe;}
        QHash<QString, double*> getHash(){return hash ;}

    private:
        QHash<QString, double*> hash;
        QStringList Titles;
        QList<Dataref> DListe;
};

#endif // GLOBALDATA_H

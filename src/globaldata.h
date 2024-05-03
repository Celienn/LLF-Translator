#ifndef GLOBALDATA_H
#define GLOBALDATA_H

#include <QHash>
#include <QHashIterator>
#include <QString>
#include <mainwindow.h>
#include <QList>
#include "ui_mainwindow.h"
#include "dataref.h"
#include <QStringList>

class GlobalData {

    public:
        GlobalData(Ui::MainWindow* parent = nullptr);
        void trackVariable(QString key, double* value);
        void unTrackVariable(QString key);
        void initCollumn();
        void initData(QList<Dataref*> DListe);
        void setcase(int i, int row, QString text);
        double getValue(QString key);
        QHash<QString, double*> getHash(){ return hash; };

    private:
        QHash<QString, double*> hash;
        QStringList Titles;
        Ui::MainWindow* parent;
};

#endif // GLOBALDATA_H

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
        void initCollumn();
        void initData(QList<Dataref*> DListe);
        void setcase(int i, int row, QString text);
        void removeFromList(const char * key);
        void addPointers(QList<Dataref*> DListe);
        QHash<QString, double*> getPointers(){ return pts; };
        Ui::MainWindow* getParent() { return parent; };

    private:
        QStringList Titles;
        Ui::MainWindow* parent;
        QHash<QString, double*> pts;
};

#endif // GLOBALDATA_H

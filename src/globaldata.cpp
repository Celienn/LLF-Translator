#include "globaldata.h"
#include <qdebug.h>
#include "ui_mainwindow.h"
#include "QTableWidgetItem"
#include "src/dataref.h"

GlobalData::GlobalData(Ui::MainWindow* parent)
{
    Titles << "Name" << "Value" << "Unit" << "Frequency";
    this->parent = parent;
}

void GlobalData::trackVariable(QString key, double* value){

    hash[key] = value;

}

void GlobalData::unTrackVariable(QString key){

    if (hash.contains(key)) hash.remove(key);

}

double GlobalData::getValue(QString key){

    return (hash.contains(key) ? *(hash[key]) : 0);

}


void GlobalData::setcase(int i , int row, QString text){
    QTableWidgetItem *pCell = parent->Table->item(0, 0);
    pCell = new QTableWidgetItem;
    parent->Table->setItem(i, row, pCell);
    pCell->setText(text);
}


void GlobalData::initData(QList<Dataref*> DListe){

    qDebug() << DListe.size();
    QTableWidgetItem *pCell = parent->Table->item(0, 0);
    for (int i = 0; i < DListe.size(); ++i) {

        setcase(i,0,DListe[i]->name);

        setcase(i,1,QString::number(DListe[i]->value));

        setcase(i,2,DListe[i]->unit);

        setcase(i,3,QString::number(DListe[i]->frequency));

    }
}

void GlobalData::initCollumn(){
    parent->Table->setColumnCount(Titles.size());
    parent->Table->setHorizontalHeaderLabels(Titles);
}

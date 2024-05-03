#include "globaldata.h"
#include <qdebug.h>
#include "ui_mainwindow.h"
#include "QTableWidgetItem"
#include "src/dataref.h"

GlobalData::GlobalData()
{
    Titles << "Name" << "Value" << "Unit" << "Frequency";

    Dataref d1("d1",1,10,"m/s");
    Dataref d2("d2",2,20,"radian");
    DListe.append(d1);
    DListe.append(d2);

    qDebug() << DListe.size() << "Init";

    /*double var = 0.152;

    hash["oui"] = &var;
    qDebug() << *hash["oui"];*/
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


void GlobalData::setcase(Ui::MainWindow *ui,int i , int row, QString text){
    QTableWidgetItem *pCell = ui->Table->item(0, 0);
    pCell = new QTableWidgetItem;
    ui->Table->setItem(i, row, pCell);
    pCell->setText(text);
}


void GlobalData::initData(Ui::MainWindow *ui){

    qDebug() << DListe.size();
    QTableWidgetItem *pCell = ui->Table->item(0, 0);
    for (int i = 0; i < DListe.size(); ++i) {

        setcase(ui,i,0,DListe[i].getName());

        setcase(ui,i,1,QString::number(DListe[i].getValue()));

        setcase(ui,i,2,DListe[i].getUnit());

        setcase(ui,i,3,QString::number(DListe[i].getFrequency()));

    }
}

void GlobalData::initCollumn(Ui::MainWindow *ui){
    ui->Table->setColumnCount(Titles.size());
    ui->Table->setHorizontalHeaderLabels(Titles);

    initData(ui);

    /*QTableWidgetItem *pCell = ui->Table->item(0, 0);
    for (int i = 0; i < Titles.size(); ++i) {

        pCell = new QTableWidgetItem;
        ui->Table->setItem(0, i, pCell);
        pCell->setText(Titles[i]);

    }*/
}

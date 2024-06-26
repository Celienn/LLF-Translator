#include "globaldata.h"

GlobalData::GlobalData(Ui::MainWindow* parent)
{
    Titles << "Name" << "Value" << "Unit" << "Frequency";
    this->parent = parent;
    initCollumn();
}

void GlobalData::removeFromList(const char * key){
    QString keyString = QString::fromUtf8(key);
    int index = -1;
    for(int i = 0; i < parent->listWidget->count(); ++i){
        if(parent->listWidget->item(i)->text() == keyString){
            index = i;
            break;
        }
    }
    if(index != -1){
        QListWidgetItem *item = parent->listWidget->takeItem(index);
        delete item;
    }
}

void GlobalData::setcase(int i , int row, QString text){
    QTableWidgetItem *pCell = parent->Table->item(0, 0);
    pCell = new QTableWidgetItem;
    parent->Table->setItem(i, row, pCell);
    pCell->setText(text);
}

// Todo : optimize :p
void GlobalData::initData(QList<Dataref*> DListe){
    QTableWidgetItem *pCell = parent->Table->item(0, 0);
    parent->Table->clearContents();
    parent->Table->setRowCount(DListe.size()); 

    for (int i = 0; i < DListe.size(); ++i) {

        setcase(i,0,DListe[i]->MSFSvar);
        setcase(i,1,QString::number(DListe[i]->value));
        setcase(i,2,DListe[i]->unit);
        setcase(i,3,QString::number(DListe[i]->frequency));
    }
}

void GlobalData::initCollumn(){
    parent->Table->setColumnCount(Titles.size());
    parent->Table->setHorizontalHeaderLabels(Titles);
}

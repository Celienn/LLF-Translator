#include "src/mainwindow.h"
#include "ui_mainwindow.h"
#include "src/graphicalview.h"
#include <iostream>
#include <QCoreApplication>
#include <QHostAddress>
#include <src/dataref.h>

void MainWindow::init()
{
    ui->setupUi(this);
    ui->Table->setRowCount(23);
    ui->Table->setColumnCount(0);
    ui->Table->setColumnWidth(0,143);
    //ui->Table->horizontalHeader()->hide();
    //ui->Table->verticalHeader()->setVerticalHeaderLabels();

    QTableWidgetItem *pCell = ui->Table->item(0, 0);
    pCell = new QTableWidgetItem;
    ui->Table->setItem(0, 0, pCell);

    for (const QHostAddress &address : QNetworkInterface::allAddresses())
    {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
        {
            ui->label->setText(address.toString());
        }

    }

}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    init();
    Data = new GlobalData(ui);
}

MainWindow::MainWindow(LLFTranslator *translator, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) 
{
    m_LLFTranslator = translator;
    init();
    Data = new GlobalData(ui);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    int value = 10;

    QString itemname = item->text().remove("\"");
    qDebug() << itemname.toStdString();
    graphicalview *GraphicalView = new graphicalview(this,itemname.toStdString(),&value);
    // Show the second window
    GraphicalView->show();
}


void MainWindow::on_pushButton_clicked()
{
    ui->listWidget->clear();

    //QTableWidgetItem *pCell = ui->Table->item(0, 0);

    //double var = 452.0;

    GlobalData* Data = new GlobalData(ui);

    Data->initCollumn();

/*
    for (int i = 1;i < 23 ; i++)
    {
        QString IStr = QString::number(i);
        qDebug() << obj->getValue(IStr);

      QHashIterator<QString, double*> k(obj->getHash());
        while (k.hasNext()) {
            k.next();
            qDebug() << qPrintable(k.key()) << ": " << *k.value();
            ui->listWidget->addItem(k.key());
        }

        pCell = new QTableWidgetItem;
        ui->Table->setItem(i, 0, pCell);
        pCell->setText(IStr);
    }
*/


    QHashIterator<QString, double*> i(Data->getHash());
    while (i.hasNext()) {
        i.next();
        qDebug() << qPrintable(i.key()) << ": " << *i.value();
        ui->listWidget->addItem(i.key());
    }
}

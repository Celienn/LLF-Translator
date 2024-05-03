#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graphicalview.h"
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
    QString itemname = item->text();

    double* ptr = Data->getPointers()[itemname];
    if (ptr == nullptr) return;
    
    graphicalview *GraphicalView = new graphicalview(this,itemname,ptr);
    GraphicalView->show();
}


void MainWindow::on_pushButton_clicked()
{
    GlobalData* Data = new GlobalData(ui);
    Data->initCollumn();
}

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graphicalview.h"
#include "globaldata.h"
#include <iostream>
#include <QCoreApplication>
#include <QHostAddress>

void MainWindow::init()
{
    ui->setupUi(this);
    ui->Table->setRowCount(23);
    ui->Table->setColumnCount(7);
    ui->Table->setColumnWidth(0,143);
    ui->Table->horizontalHeader()->hide();
    ui->Table->verticalHeader()->hide();

    QTableWidgetItem *pCell = ui->Table->item(0, 0);
    pCell = new QTableWidgetItem;
    ui->Table->setItem(0, 0, pCell);
    pCell->setText("test");
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
}

MainWindow::MainWindow(LLFTranslator *translator, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) 
{
    m_LLFTranslator = translator;
    init();
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

    QTableWidgetItem *pCell = ui->Table->item(0, 0);

    double var = 452.0;
    GlobalData* obj = new GlobalData();
    obj->trackVariable("test", &var);

    obj->trackVariable("altimeter",&var);

    /*auto allData = GlobalData::getInstance().getAllData();
    for (const auto& pair : allData) {
        std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
        ui->listWidget->addItem(QString::fromStdString(pair.first));
    }*/

    for (int i = 1;i < 23 ; i++)
    {
        QString IStr = QString::number(i);
        qDebug() << obj->getValue(IStr);

/*        QHashIterator<QString, double*> k(obj->getHash());
        while (k.hasNext()) {
            k.next();
            qDebug() << qPrintable(k.key()) << ": " << *k.value();
            ui->listWidget->addItem(k.key());
        }
*/
        pCell = new QTableWidgetItem;
        ui->Table->setItem(i, 0, pCell);
        pCell->setText(IStr);
    }



    QHashIterator<QString, double*> i(obj->getHash());
    while (i.hasNext()) {
        i.next();
        qDebug() << qPrintable(i.key()) << ": " << *i.value();
        ui->listWidget->addItem(i.key());
    }
}

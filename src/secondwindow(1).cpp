#include "secondwindow.h"
#include "ui_secondwindow.h"
#include "globaldata.h"
#include <QPainter>
secondwindow::secondwindow(QWidget *parent,std::string id)
    : QDialog(parent)
    , ui(new Ui::secondwindow)
{
    ui->setupUi(this);

}

secondwindow::~secondwindow()
{
    delete ui;
}

void secondwindow::paintEvent(QPaintEvent *event)
{
    QPainter mytext(this);
    mytext.drawText(QPoint(20,30),"Qt5test");

    QPainter myline(this);
    myline.drawLine(50,100,75,width());
    qDebug() << width();
    qDebug() << height();
}


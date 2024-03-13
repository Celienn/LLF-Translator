#include "mainwindow.h"
#include "llftranslator.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    LLFTranslator llf;
    llf.connect();
    // llf.readVar<double>(llf.translateXPlaneToMFS("sim/cockpit2/gauges/indicators/altitude_ft_pilot"), "meter", SIMCONNECT_DATATYPE_FLOAT64,[](double altitude){
    //     qDebug() << altitude;
    // },5);
    
    return a.exec();
}

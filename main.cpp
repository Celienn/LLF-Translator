#include "mainwindow.h"
#include "llftranslator.h"

#include <QApplication>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    LLFTranslator llf;
    llf.connect();
    llf.readVar("Plane Altitude", "feet", SIMCONNECT_DATATYPE_FLOAT64,5);
    return a.exec();
}

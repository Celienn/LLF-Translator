#include "mainwindow.h"
#include "llftranslator.h"
#include <QApplication>
#include <QDebug>
#include "globaldata.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    LLFTranslator llf;
    llf.connect();

    return a.exec();
}

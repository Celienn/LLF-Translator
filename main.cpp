#include "src/mainwindow.h"
#include "src/llftranslator.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    LLFTranslator llf;
    llf.connect();
    return a.exec();
}

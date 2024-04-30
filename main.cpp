#include "src/mainwindow.h"
#include <QApplication>
#include "src/globaldata.h"
#include <random>
#include <iostream>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.setWindowTitle("LLF Translator Debugger");
    std::random_device rd;
    std::mt19937 gen(rd());

    return a.exec();
}

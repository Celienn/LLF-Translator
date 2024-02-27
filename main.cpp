#include "llftranslator.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LLFTranslator w;
    w.show();
    return a.exec();
}

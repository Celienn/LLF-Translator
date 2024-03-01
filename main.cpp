#include "mainwindow.h"
#include "llftranslator.h"

#include <QApplication>

vector<string> split(string str, string delimiter){
    size_t pos = 0;
    string token;
    vector<string> tokens;

    while ((pos = str.find(delimiter)) != string::npos) {
        token = str.substr(0, pos);
        tokens.push_back(token);
        str.erase(0, pos + delimiter.length());
    }

    tokens.push_back(str);
    return tokens;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    LLFTranslator llf;
    llf.connect();
    llf.readVar();
    return a.exec();
}

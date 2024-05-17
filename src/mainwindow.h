#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QListWidgetItem>
#include <QMainWindow>
#include <string>
#include <QtNetwork>
#include <QtCore>
#include "ui_mainwindow.h"
#include "graphicalview.h"
#include <iostream>
#include <QCoreApplication>
#include <QHostAddress>
#include <src/dataref.h>
#include <QtGlobal>
#include <qledlabel.h>
#include <llftranslator.h>
#include <QTextCursor>

class LLFTranslator;
class GlobalData;

QT_BEGIN_NAMESPACE

namespace Ui {
    class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow(LLFTranslator *translator, QWidget *parent = nullptr);
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();
        friend void DebugOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

        GlobalData* Data;
        
    private slots:
        void on_listWidget_itemClicked(QListWidgetItem *item);
        void onConnectButtonClicked();
    private:
        Ui::MainWindow *ui;
        LLFTranslator *m_LLFTranslator;

        void init();
};

extern MainWindow *g_mainWindow;

#endif // MAINWINDOW_H

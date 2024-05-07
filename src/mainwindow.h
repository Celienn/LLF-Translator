#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QListWidgetItem>
#include <QMainWindow>
#include <string>
#include <QtNetwork>
#include <QtCore>

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
        
        GlobalData* Data;
        
    private slots:
        void on_listWidget_itemClicked(QListWidgetItem *item);

    private:
        Ui::MainWindow *ui;
        LLFTranslator *m_LLFTranslator;

        void init();

};

#endif // MAINWINDOW_H

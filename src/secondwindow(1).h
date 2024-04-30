#ifndef SECONDWINDOW_H
#define SECONDWINDOW_H

#include <QDialog>
#include <QPainter>
namespace Ui {
class secondwindow;
}

class secondwindow : public QDialog
{
    Q_OBJECT

public:
    explicit secondwindow(QWidget *parent = nullptr, std::string id = "caca");
    ~secondwindow();
    virtual void paintEvent(QPaintEvent *event);
private:
    Ui::secondwindow *ui;
};

#endif // SECONDWINDOW_H

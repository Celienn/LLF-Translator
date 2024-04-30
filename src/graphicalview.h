#ifndef GRAPHICALVIEW_H
#define GRAPHICALVIEW_H

#include <QDialog>
#include <QPainter>
#include "circularbuffer.h"
#include <QTimer>
namespace Ui {
class secondwindow;
}

class secondwindow : public QDialog
{
    Q_OBJECT

    public:
        explicit secondwindow(QWidget *parent = nullptr, std::string id = "caca" , int* pointer = nullptr);
        ~secondwindow();
        virtual void paintEvent(QPaintEvent *event);
        void repere(double xmin,double xmax,double ymin,double ymax, double xpas,double ypas);
        void drawPoints(CircularBuffer* buffer,double xmin,double xmax,double ymin,double ymax);
        void cubicBezier(QPainter& paint, QList<QPoint>& pts );
        void traceCourbe(QPainter& paint, QList<QPoint>& points );
        void drawCross(QPainter* painter, QPoint pos, int* value,int i);

    private slots:
        void Timer_Timeout_Event_Slot();
    private:
        Ui::secondwindow *ui;
        CircularBuffer* buffer;
        QTimer *Timer;
        int accumulatetest;
        QPoint mousepos;
        int* vptr;
    protected:
        void mouseMoveEvent(QMouseEvent *event);
};

/*typedef struct
{
    double min;
    double max;
    float pas;
    int inc;
    char label[512];
}t_axe;*/

#endif // GRAPHICALVIEW_H

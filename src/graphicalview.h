#ifndef GRAPHICALVIEW_H
#define GRAPHICALVIEW_H

#include <QDialog>
#include <QPainter>
#include "circularbuffer.h"
#include <QTimer>
#include "ui_graphicalview.h"
#include "globaldata.h"
#include <QPainter>
#include <QPoint>
#include <QMouseEvent>
#include <QRandomGenerator>
#include "circularbuffer.h"
#include <cmath>
#include <QBrush>

#define LARGEURVUE 1000
#define HAUTEURVUE 1000
#define nbpoints 50
#define RESOLUTION 35
#define DELAY 50 // delay between  value asking.


namespace Ui {
    class graphicalview;
}

class graphicalview : public QDialog
{
    Q_OBJECT

    public:
        explicit graphicalview(QWidget *parent = nullptr, std::string id = "caca" , int* pointer = nullptr);
        ~graphicalview();
        virtual void paintEvent(QPaintEvent *event);
        void repere(double xmin,double xmax,double ymin,double ymax, double xpas,double ypas);
        void drawPoints(CircularBuffer* buffer,double xmin,double xmax,double ymin,double ymax);
        void cubicBezier(QPainter& paint, QList<QPoint>& pts );
        void traceCourbe(QPainter& paint, QList<QPoint>& points );
        void drawCross(QPainter* painter, QPoint pos, int* value,int i);

    private slots:
        void Timer_Timeout_Event_Slot();
    private:
        Ui::graphicalview *ui;
        CircularBuffer* buffer;
        QTimer *Timer;
        QPoint mousepos;
        int accumulatetest;
        int* vptr;
        int count;
        int timercount;
        int fps;
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

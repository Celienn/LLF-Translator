#ifndef GRAPHICALVIEW_H
#define GRAPHICALVIEW_H

#include <QDialog>
#include <QPainter>
#include <QString>
#include "circularbuffer.h"
#include <QTimer>
#include "ui_graphicalview.h"
#include "globaldata.h"
#include <QPainter>
#include <QPoint>
#include <QMouseEvent>
#include <QRandomGenerator>
#include <cmath>
#include <QBrush>

#define LARGEURVUE 1000
#define HAUTEURVUE 1000
#define nbpoints 100
#define RESOLUTION 10
#define DELAY 50 

// TODO : dynamic scale

namespace Ui {
    class graphicalview;
}

class graphicalview : public QDialog
{
    Q_OBJECT

    public:
        explicit graphicalview(QWidget *parent, QString id, double* pointer);
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
        double* vptr;
        int count;
        int timercount;
        int fps;
    protected:
        void mouseMoveEvent(QMouseEvent *event);
};

#endif // GRAPHICALVIEW_H

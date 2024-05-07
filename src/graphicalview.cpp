#include "graphicalview.h"

graphicalview::graphicalview(QWidget *parent, QString id, double* pointer)
    : QDialog(parent)
    , ui(new Ui::graphicalview)
{
    ui->setupUi(this);
    buffer = new CircularBuffer(nbpoints);
    Timer = new QTimer(this);
    connect(Timer,SIGNAL(timeout()),this,SLOT(Timer_Timeout_Event_Slot()));
    Timer->setInterval(DELAY);
    Timer->start();
    this->setWindowTitle(id);
    accumulatetest = 0;
    setMouseTracking(true);
    vptr = pointer;
}

graphicalview::~graphicalview()
{
    delete ui;
}

void graphicalview::mouseMoveEvent(QMouseEvent *event)
{
    mousepos = event->pos();
}

void graphicalview::Timer_Timeout_Event_Slot()
{
    double var = 600.0;
    buffer->insertnewvalue(*vptr);
}

void graphicalview::cubicBezier(QPainter& paint, QList<QPoint>& pts )
{
    paint.save() ;
    paint.setPen( QPen(Qt::red, 2 ) ) ;
    QPoint ptAct, ptPrec = pts[0] ;

    int n = pts.size() - 1 ;
    for ( int i = 0 ; i <= RESOLUTION ; i++ ) {

        double t = i / (double)RESOLUTION ;
        double x = 0.0 , y = 0.0 ;
        double a, b, c, d ;

        switch( n ) {
        case 1 :
            a = 1 - t ;
            b = t ;
            x = pts[0].x() * a + pts[1].x() * b ;
            y = pts[0].y() * a + pts[1].y() * b ;
            break ;

        case 2 :
            a = pow(1 - t, 2 ) ;
            b = 2 * t * ( 1 - t ) ;
            c = pow(t, 2 ) ;
            x = pts[0].x() * a + pts[1].x() * b + pts[2].x() * c ;
            y = pts[0].y() * a + pts[1].y() * b + pts[2].y() * c ;
            break ;

        default :
            a = pow(1 - t, 3 ) ;
            b = 3 * t * pow(1 - t, 2 ) ;
            c = 3 * pow(t, 2 ) * ( 1 - t ) ;
            d = pow(t, 3 ) ;
            x = pts[0].x() * a + pts[1].x() * b + pts[2].x() * c + pts[3].x() * d ;
            y = pts[0].y() * a + pts[1].y() * b + pts[2].y() * c + pts[3].y() * d ;
            break ;

        }

        ptAct.setX( (int)x ) ;
        ptAct.setY( (int)y ) ;

        paint.drawPoint(ptAct ) ;
        paint.drawLine(ptPrec, ptAct ) ;

        ptPrec = ptAct ;
    }

    paint.restore() ;
}

void graphicalview::traceCourbe(QPainter& paint, QList<QPoint>& points )
{
    for ( int i = 0; i < points.size() - 2 ; i+=2 ){
        QList<QPoint> pts;
        pts << points[i] << points[i+1] << points[i+2];
        cubicBezier(paint,pts );
    }
}



void graphicalview::repere(double xmin,double xmax,double ymin,double ymax, double xpas,double ypas) {
    QPainter myline(this);
    double offsetX = (width()/(fabs(xmin)+fabs(xmax)))*fabs(xmin);
    double offsetY = (height()/(fabs(ymin)+fabs(ymax)))*fabs(ymax);

    myline.drawLine(offsetX,0,offsetX,height());
    myline.drawLine(0,offsetY,width(),offsetY);

    double gradx = (fabs(xmin) + fabs(xmax))*xpas;
    for (int i=0;i<gradx+1	;i++){
        if (i == fabs(xmin)*xpas) continue;
        float calc = (width()/gradx)*i;
        myline.drawRect((calc)-1, (offsetY)-5,2, 10);
    }

    gradx = (fabs(xmin) + fabs(xmax));
    for (int i=0;i<gradx+1;i++){
        if (i == fabs(xmin)) continue;
        if (abs(i)%1) continue;
        if (i == 0) continue;
        if(i == gradx)continue;
    }


    float grady = (fabs(ymin) + fabs(ymax))*ypas;
    grady = 30; // temp
    for (int i=0;i<grady+1	;i++){
        if (i == abs(ymin)*ypas) continue;
        float calc = (height()/grady)*i;
        myline.drawRect(offsetX-5, height()-(calc)-1,10, 2);
    }

    grady = (fabs(ymin) + fabs(ymax));
    for (int i=0;i<grady+1	;i++){
        if (i == abs(ymin)) continue;
        if (abs(i)%1) continue;
        if (i == 0) continue;
        if(i == grady)continue;
    }
}

void graphicalview::drawCross(QPainter* painter, QPoint pos, int* value,int i)
{
    painter->drawText(QPoint(pos.x()-7,pos.y()-10),QString::number(*value));
    painter->drawText(QPoint(pos.x()-5,pos.y()-24),QString::number(i));
    painter->setPen(Qt::gray);
    painter->drawLine(QPoint(0,pos.y()), QPoint(width(),pos.y())); // x axes
    painter->drawLine(QPoint(pos.x(),0), QPoint(pos.x(),height())); // y axes
}

void graphicalview::drawPoints(CircularBuffer* buffer,double xmin,double xmax,double ymin,double ymax)
{
    QPainter paint( this );
    float gradx = (fabs(xmin) + fabs(xmax));
    float grady = (fabs(ymin) + fabs(ymax));

    gradx = (float)(width())/gradx;
    grady = (float)(height())/grady;

    QPainter myline(this);
    QPoint lastpos = QPoint();
    QList<QPoint> points;
    bool crossed = false;
    for (int i = 0; i<buffer->getSize();i++){
        myline.setPen(Qt::white);
        int value = buffer->getValue(i);
        QPoint pos = QPoint(width()-gradx*(i+1)-5,(fabs(ymax)*grady)-grady * value-5);
        points << pos;

        if (abs(mousepos.x() - pos.x()) < gradx && !crossed){

            drawCross(&paint, pos, &value, i);
            crossed = true; // s'assurer que la croix ne s'affiche qu'une seule fois
        }

        traceCourbe(paint, points);
        lastpos = pos;
    }
    traceCourbe(paint, points);
}

void graphicalview::paintEvent(QPaintEvent *event)
{
    int maxvalue = buffer->getMax() * 1.2;
    repere(buffer->getSize(),1,1,maxvalue, 1,1);
    drawPoints(buffer,1,buffer->getSize(),1,maxvalue);
    update();

}


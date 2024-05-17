#ifndef LEDINDICATOR_H
#define LEDINDICATOR_H

#include <qledlabel.h>

static const int SIZE = 20;
static const QString greenSS = QString("color: white;border-radius: %1;background-color: qlineargradient(spread:pad, x1:0.145, y1:0.16, x2:1, y2:1, stop:0 rgba(20, 252, 7, 255), stop:1 rgba(25, 134, 5, 255));").arg(SIZE/2);
static const QString yellowSS = QString("color: white;border-radius: %1;background-color: qlineargradient(spread:pad, x1:0.232, y1:0.272, x2:0.98, y2:0.959773, stop:0 rgba(255, 113, 4, 255), stop:1 rgba(91, 41, 7, 255))").arg(SIZE/2);
static const QString redSS = QString("color: white;border-radius: %1;background-color: qlineargradient(spread:pad, x1:0.145, y1:0.16, x2:0.92, y2:0.988636, stop:0 rgba(255, 12, 12, 255), stop:0.869347 rgba(103, 0, 0, 255));").arg(SIZE/2);

QLedLabel::QLedLabel(QWidget *parent) : QLabel(parent) {
    setState(Red);
    setFixedSize(SIZE, SIZE);
}

QLedLabel::~QLedLabel() {
    
}

void QLedLabel::setState(Color color) {
    switch (color)
    {
        case Green:
            setStyleSheet(greenSS);
            break;
        
        case Yellow:
            setStyleSheet(yellowSS);
            break;
        
        case Red:
            setStyleSheet(redSS);
            break;
        
        default:
            break;
    }
}

#endif // LEDINDICATOR_H
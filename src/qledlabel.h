#ifndef QLEDLABEL_H
#define QLEDLABEL_H

#include <QLabel>
#include <QWidget>

enum Color { Green, Yellow, Red };

class QLedLabel : public QLabel
{

    public:
        QLedLabel(QWidget *parent = nullptr);
        ~QLedLabel();
        void setState(Color color);
};

#endif // QLEDLABEL_H
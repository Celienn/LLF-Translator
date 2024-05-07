#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <QVector>

class CircularBuffer {
    private:
        int size;
        QVector<double> buffer;
        int head;
        double max = 0.;

    public:
        CircularBuffer(int size) : size(size), buffer(size), head(0){};
        void insertnewvalue(double newValue);
        int getSize(){ return buffer.size(); };
        int getValue(int index){ return buffer[index]; };
        double getMax(){ return max; }
};

#endif // CIRCULARBUFFER_H

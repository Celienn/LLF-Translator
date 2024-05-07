#include "circularbuffer.h"

void CircularBuffer::insertnewvalue(double newValue) {
    max = (max < newValue) ? newValue : max;
    buffer.insert(buffer.begin(), newValue);
    buffer.pop_back();
}

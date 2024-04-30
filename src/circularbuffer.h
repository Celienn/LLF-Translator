#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <iostream>
#include <vector>

class CircularBuffer {
private:
    int size;
    std::vector<int> buffer;
    int head;

public:
    CircularBuffer(int size) : size(size), buffer(size), head(0){}

    void insertnewvalue(int newValue) {
        // Insert the new value at the beginning
        buffer.insert(buffer.begin(), newValue);
        // Remove the last element
        buffer.pop_back();
    }


    int getSize()
    {
        return buffer.size();
    }

    int getValue(int index)
    {
        return buffer[index];
    }

    void printBuffer() {
        std::cout << "Current buffer: ";
        for (int i = 0; i < size; ++i) {
            std::cout << buffer[i] << " ";
        }
        std::cout << std::endl;
    }
};
#endif // CIRCULARBUFFER_H

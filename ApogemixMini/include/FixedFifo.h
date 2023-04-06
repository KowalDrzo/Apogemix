#ifndef FIXED_FIFO_H
#define FIXED_FIFO_H

template <typename T, const int MaxLen>
class FixedFifo {

    T dataQue[MaxLen];
    uint8_t len = 0;
    uint8_t writeIndex = 0;
    uint8_t readIndex = 0;

public:

    void push(const T &data) {

        if (len >= MaxLen) pop();

        dataQue[writeIndex] = data;
        writeIndex = (writeIndex + 1) % MaxLen;
        len++;
    }

    T pop() {

        T data = dataQue[readIndex];
        memset((uint8_t*) &dataQue[readIndex], 0, sizeof(T));
        readIndex = (readIndex + 1) % MaxLen;
        len--;

        return data;
    }

    uint8_t size() {
        return len;
    }
};

#endif
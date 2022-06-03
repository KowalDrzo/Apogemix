#ifndef FIXED_FIFO_H
#define FIXED_FIFO_H

#include <queue>

template <typename T, int MaxLen, typename Container=std::deque<T>>
class FixedFifo : public std::queue<T, Container> {
public:
    void push(const T& value) {
        if (this->size() == MaxLen) {
           this->c.pop_front();
        }
        std::queue<T, Container>::push(value);
    }
};


#endif
#ifndef SOIL_UTIL_DEQUE_H
#define SOIL_UTIL_DEQUE_H
#include <cstring>
#include <stdexcept>
#include <vector>

// TODO performance sucks
namespace soil::util {
    template <class T>
    class Deque {
    public:
        explicit Deque(int capacity = 8) : ni(0), nextFreeSlot(0) {
            buffer_.resize(capacity);
        }

        ~Deque() = default;

        void Push(T e) {
            auto bufferSize = buffer_.size();
            if (nextFreeSlot == bufferSize) { // enough space?
                if (ni == 0) {
                    auto newBufferSize = bufferSize * 2;
                    buffer_.resize(newBufferSize);
                    bufferSize = newBufferSize;
                } else {
                    if (ni + 1 == bufferSize) {
                        nextFreeSlot = 1;
                        ni = 0;
                    } else {
                        std::memcpy(buffer_.data(), &buffer_[ni + 1], (bufferSize - ni - 1) * sizeof(T));
                        nextFreeSlot = bufferSize - ni - 1;
                        ni = 0;
                    }
                }
            }
            buffer_[nextFreeSlot++] = e;
        }

        T Pop() {
            /* if (ni == nextFreeSlot) {
                 throw std::out_of_range("Deque<T>::Pop()");
             }*/
            return buffer_[ni++];
        }

        bool Empty() const {
            return ni == nextFreeSlot;
        }

    private:
        std::vector<T> buffer_;
        int ni;
        int nextFreeSlot;
    };
} // namespace soil::util

#endif // SOIL_UTIL_DEQUE_H

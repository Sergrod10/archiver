#pragma once

#include <functional>
#include <stdexcept>

template <typename T, typename Compare = std::less<T>>
class PriorityQueue {
public:
    PriorityQueue() {
        comp_ = Compare();
        n_ = 0;
    }

    void Push(T x) {
        heap_.push_back(x);
        n_++;
        SiftUp(n_ - 1);
    }

    void Pop() {
        if (!Size()) {
            throw std::runtime_error("priority queue: popped from empty array");
        }
        std::swap(heap_[0], heap_[n_ - 1]);
        n_--;
        heap_.pop_back();
        SiftDown();
    }

    T Top() const {
        if (!Size()) {
            throw std::runtime_error("priority queue: executed top from empty array");
        }
        return heap_[0];
    }

    int Size() const {
        return n_;
    }

private:
    Compare comp_;
    std::vector<T> heap_;
    int n_;

    void SiftDown() {
        int now = 0;
        while (2 * now + 1 < n_) {
            if (2 * now + 2 < n_) {
                int to_swap = 2 * now + 2;
                if (!comp_(heap_[2 * now + 1], heap_[2 * now + 2])) {
                    to_swap = 2 * now + 1;
                }
                if (!comp_(heap_[now], heap_[to_swap])) {
                    break;
                }
                std::swap(heap_[now], heap_[to_swap]);
                now = to_swap;
                continue;
            }
            if (comp_(heap_[now], heap_[2 * now + 1])) {
                std::swap(heap_[now], heap_[2 * now + 1]);
                now = 2 * now + 1;
            } else {
                break;
            }
        }
    }

    void SiftUp(int now) {
        while (now > 0) {
            int parent = (now - 1) / 2;
            if (comp_(heap_[parent], heap_[now])) {
                std::swap(heap_[parent], heap_[now]);
                now = parent;
            } else {
                break;
            }
        }
    }
};
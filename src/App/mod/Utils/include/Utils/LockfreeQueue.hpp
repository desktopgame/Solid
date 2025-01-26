#pragma once
#include <mutex>
#include <queue>
#include <thread>
// see: https://qiita.com/kumagi/items/4224282d95804d46fc39

namespace Lib::Utils {
template <typename T>
class LockfreeQueue {
public:
    explicit LockfreeQueue(size_t capacity)
        : buffer_(capacity)
        , head_(0)
        , tail_(0)
    {
    }

    bool enqueue(const T& item)
    {
        size_t currentTail = tail_.load(std::memory_order_relaxed);
        size_t nextTail = (currentTail + 1) % buffer_.size();
        if (nextTail == head_.load(std::memory_order_acquire)) {
            // キューが満杯
            return false;
        }
        buffer_[currentTail] = item;
        tail_.store(nextTail, std::memory_order_release);
        return true;
    }

    bool dequeue(T& item)
    {
        size_t currentHead = head_.load(std::memory_order_relaxed);
        if (currentHead == tail_.load(std::memory_order_acquire)) {
            // キューが空
            return false;
        }
        item = buffer_[currentHead];
        head_.store((currentHead + 1) % buffer_.size(), std::memory_order_release);
        return true;
    }

private:
    std::vector<T> buffer_;
    std::atomic<size_t> head_;
    std::atomic<size_t> tail_;
};
}
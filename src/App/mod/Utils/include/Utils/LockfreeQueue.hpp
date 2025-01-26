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
        : m_buffer(capacity)
        , m_head(0)
        , m_tail(0)
    {
    }

    bool enqueue(const T& item)
    {
        size_t currentTail = m_tail.load(std::memory_order_relaxed);
        size_t nextTail = (currentTail + 1) % m_buffer.size();
        if (nextTail == m_head.load(std::memory_order_acquire)) {
            // キューが満杯
            return false;
        }
        m_buffer[currentTail] = item;
        m_tail.store(nextTail, std::memory_order_release);
        return true;
    }

    bool dequeue(T& item)
    {
        size_t currentHead = m_head.load(std::memory_order_relaxed);
        if (currentHead == m_tail.load(std::memory_order_acquire)) {
            // キューが空
            return false;
        }
        item = m_buffer[currentHead];
        m_head.store((currentHead + 1) % m_buffer.size(), std::memory_order_release);
        return true;
    }

private:
    std::vector<T> m_buffer;
    std::atomic<size_t> m_head;
    std::atomic<size_t> m_tail;
};
}
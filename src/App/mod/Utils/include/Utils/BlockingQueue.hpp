#pragma once
#include <mutex>
#include <queue>
#include <thread>
// see: https://qiita.com/kumagi/items/4224282d95804d46fc39

namespace Lib::Utils {
template <typename T>
class BlockingQueue {
public:
    explicit BlockingQueue()
        : m_mutex()
        , m_condVar()
        , m_queue()
    {
    }

    void enqueue(const T& item)
    {
        std::lock_guard<std::mutex> guard(m_mutex);
        bool wasEmpty = m_queue.empty();
        m_queue.push(item);
        if (wasEmpty) {
            m_condVar.notify_all();
        }
    }

    T dequeue()
    {
        while (true) {
            std::unique_lock<std::mutex> guard(m_mutex);
            if (m_queue.empty()) {
                m_condVar.wait(guard);
            }
            if (m_queue.empty()) {
                continue;
            }
            T result = m_queue.front();
            m_queue.pop();
            return result;
        }
    }

    size_t size() const
    {
        std::lock_guard<std::mutex>(m_mutex);
        return m_queue.size();
    }

private:
    mutable std::mutex m_mutex;
    mutable std::condition_variable m_condVar;
    std::queue<T> m_queue;
};
}
#pragma once

#include <boost/thread.hpp>
#include <boost/thread/lockable_adapter.hpp>

namespace mcidle {
namespace thread {

// A simple thread-safe blocking queue which
// is itself lockable
template <typename T>
class SafeQueue : public boost::basic_lockable_adapter<boost::mutex>{
    SafeQueue();

    void Push(T&& val)
    {
        boost::lock_guard<SafeQueue<T>> guard(*this);
        m_Queue.push_back(val);
    }

    T& Pop()
    {
        boost::lock_guard<SafeQueue<T>> guard(*this);
        return m_Queue.pop_front();
    }

    bool Empty() 
    {
        boost::lock_guard<SafeQueue<T>> guard(*this);
        return m_Queue.empty();
    }

    std::size_t Size()
    {
        boost::lock_guard<SafeQueue<T>> guard(*this);
        return m_Queue.size();
    }

private:
    std::deque<T> m_Queue;
};

} // ns thread
} // ns mcidle

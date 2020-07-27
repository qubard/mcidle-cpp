#pragma once

#include <boost/thread.hpp>
#include <boost/thread/lockable_adapter.hpp>

namespace mcidle {
namespace thread {

// A simple thread-safe blocking queue which
// is itself lockable
template <typename T>
class Queue : public boost::basic_lockable_adapter<boost::mutex> {
public:
    Queue(){}

    void Push(T val)
    {
        boost::lock_guard<Queue<T>> guard(*this);
        m_Queue.push_back(val);
    }

    void Push(T&& val)
    {
        boost::lock_guard<Queue<T>> guard(*this);
        m_Queue.push_back(val);
    }

    T Pop()
    {
        auto last = m_Queue.front();
        m_Queue.pop_front();
        return last;
    }

    bool Empty() 
    {
        return m_Queue.empty();
    }

    std::size_t Size()
    {
        return m_Queue.size();
    }

private:
    std::deque<T> m_Queue;
};

} // ns thread
} // ns mcidle

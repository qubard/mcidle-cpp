#include <boost/chrono.hpp>
#include <boost/thread/thread.hpp>
#include <iostream>
#include <networking/thread/Pipe.hpp>

namespace mcidle {
namespace thread {

    Pipe::Pipe(u32 rate, u32 maxSize)
        : m_RateMS(rate)
        , m_MaxSize(maxSize)
    {
    }

    Pipe::Pipe(u32 rate, u32 maxSize, std::shared_ptr<Connection> sink)
        : m_RateMS(rate)
        , m_MaxSize(maxSize)
        , m_Sink(sink)
    {
    }

    void Pipe::SetSink(std::shared_ptr<Connection> sink,
                       std::shared_ptr<Pipe> sinkPipe)
    {
        boost::lock_guard<boost::recursive_mutex> guard(m_Mutex);
        m_Sink = sink;
        m_SinkPipe = sinkPipe;
    }

    void Pipe::Push(std::shared_ptr<Packet> packet)
    {
        boost::lock_guard<boost::recursive_mutex> guard(m_Mutex);
        if (!m_Closed && m_Queue.size() < m_MaxSize)
            m_Queue.push_back(packet);
    }

    void Pipe::Open()
    {
        boost::lock_guard<boost::recursive_mutex> guard(m_Mutex);
        m_Closed = false;
    }

    void Pipe::Close()
    {
        boost::lock_guard<boost::recursive_mutex> guard(m_Mutex);
        m_Closed = true;
        m_SinkPipe->Close();
        printf("Clearing pipe of size %d\n", m_Queue.size());
        m_Queue.clear();
    }

    void Pipe::Start()
    {
        auto PIPE_RATE = boost::chrono::milliseconds(m_RateMS);
        for (;;)
        {
            while (!m_Queue.empty())
            {
                // Lock the pipe in case the sink is replaced
                boost::lock_guard<boost::recursive_mutex> guard(m_Mutex);

                // Guarantees that the queue is emptied even if sink
                // doesn't exist
                auto pkt = m_Queue.front();
                m_Queue.pop_front();

                if (m_Sink != nullptr)
                {
                    // Send each packet
                    // printf("Sent packet %x..\n", pkt->Id());
                    auto buf = pkt->RawBuffer();
                    if (!m_Sink->SendBuffer(buf))
                    {
                        m_Sink = nullptr;
                        m_SinkPipe->Close();
                        m_SinkPipe = nullptr;
                    }
                }
            }

            // We need this because if we try to acquire a lock too fast it's a heavy
            // bottleneck
            boost::this_thread::sleep_for(PIPE_RATE);
        }
    }

    void Pipe::operator()()
    {
        Start();
    }

}  // namespace thread
}  // namespace mcidle

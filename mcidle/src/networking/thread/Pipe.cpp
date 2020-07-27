#include <networking/thread/Pipe.hpp>

#include <boost/chrono.hpp>
#include <boost/thread/thread.hpp>

namespace mcidle {
namespace thread {

Pipe::Pipe(u32 rate) : m_Rate(rate)
{
}

Pipe::Pipe(u32 rate, std::shared_ptr<Connection> sink) : m_Rate(rate), m_Sink(sink)
{
}

void Pipe::SetSink(std::shared_ptr<Connection> sink)
{
    boost::lock_guard<boost::recursive_mutex> guard(m_Mutex);
    m_Sink = sink;
}

void Pipe::Push(std::shared_ptr<Packet> packet)
{
    boost::lock_guard<boost::recursive_mutex> guard(m_Mutex);
    m_Queue.Push(packet);
}

void Pipe::Start()
{
    for(;;)
    {
        auto PIPE_RATE = boost::chrono::milliseconds(m_Rate);

        while (!m_Queue.Empty())
        {
            // Lock the pipe in case the sink is replaced
            boost::lock_guard<boost::recursive_mutex> guard(m_Mutex);

            // Guarantees that the queue is emptied even if sink 
            // doesn't exist
            auto pkt = m_Queue.Pop();
            if (m_Sink != nullptr) 
            {
                // Send each packet
                printf("Trying to send packet\n");
                auto buf = pkt->RawBuffer();
                m_Sink->SendBuffer(buf);
                printf("Sent packet..\n");
            }
        }

        // We need this because if we try to acquire a lock too fast it's a heavy bottleneck
        boost::this_thread::sleep_for(PIPE_RATE);
    }
}

void Pipe::operator()()
{
    Start();
}

}
}

#include <networking/thread/Pipe.hpp>

#include <boost/chrono.hpp>
#include <boost/thread/thread.hpp>

#include <iostream>

namespace mcidle {
namespace thread {

Pipe::Pipe(u32 rate) : m_RateMS(rate)
{
}

Pipe::Pipe(u32 rate, std::shared_ptr<Connection> sink) : m_RateMS(rate), m_Sink(sink)
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
    m_Queue.push_back(packet);
}

void Pipe::Start()
{
    for(;;)
    {
        auto PIPE_RATE = boost::chrono::milliseconds(m_RateMS);

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
                if (pkt->Id() != 0x22 && pkt->Id() != 0x12 && pkt->Id() != 0xC && pkt->Id() != 0x04 && pkt->Id() != 0x36 && pkt->Id() != 0x05 && pkt->Id() != 0x06 && pkt->Id() != 0x07 && pkt->Id() != 0x0B)
                {
                    //printf("Sent packet %x..\n", pkt->Id());
                    auto buf = pkt->RawBuffer();
                    if (!m_Sink->SendBuffer(buf))
                        m_Sink = nullptr;
                }
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

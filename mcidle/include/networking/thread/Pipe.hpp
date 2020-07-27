#pragma once

#include <networking/connection/Connection.hpp>
#include <networking/thread/Queue.hpp>
#include <networking/packet/Packet.hpp>

namespace mcidle {
namespace thread {

// The role of a Pipe is simply to send already written
// packets to the connection sink so that even if
// the sink disconnects the source continues
// operating
class Pipe
{
public:
    Pipe(u32);
    Pipe(u32, std::shared_ptr<Connection>);

    void SetSink(std::shared_ptr<Connection>);
    // Push a packet from the source to the sink
    void Push(std::shared_ptr<Packet>);

    // Overload () to make this a callable object
    // and usable with boost::thread
    void operator()();
private:
    boost::recursive_mutex m_Mutex;

    // Start running the pipe
    void Start();

    std::shared_ptr<Connection> m_Sink;
    Queue<std::shared_ptr<Packet>> m_Queue;

    // The rate at which the pipe sends data
    u32 m_Rate;
};

}
} // ns mcidle

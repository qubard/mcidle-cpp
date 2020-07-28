#pragma once

#include <networking/connection/Connection.hpp>
#include <networking/thread/Queue.hpp>
#include <networking/packet/Packet.hpp>

namespace mcidle {
namespace thread {

// The role of a Pipe is simply to pump already written
// packets to the connected socket (sink) in a thread safe manner
// so that the caller doesn't have to worry about it
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
    u32 m_RateMS;
};

}
} // ns mcidle

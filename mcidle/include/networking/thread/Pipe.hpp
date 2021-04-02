#pragma once

#include <deque>
#include <networking/connection/Connection.hpp>
#include <networking/packet/Packet.hpp>

namespace mcidle
{
namespace thread
{

// The role of a Pipe is to pump already written
// packets to the connected sink in a thread safe manner
// so that the caller doesn't have to worry about it
class Pipe
{
  public:
	Pipe(u32, u32);
	Pipe(u32, u32, std::shared_ptr<Connection>);

	void SetSink(std::shared_ptr<Connection>, std::shared_ptr<Pipe>);
	// Push a packet from the source to the sink
	void Push(std::shared_ptr<Packet>);

	void Open();
	void Close();
	// Overload () to make this a callable object
	// and usable with boost::thread
	void operator()();

  private:
	boost::recursive_mutex m_Mutex;

	// Start running the pipe
	void Start();

	std::shared_ptr<Connection> m_Sink;
	std::deque<std::shared_ptr<Packet>> m_Queue;
	// We need a reference to the sink that the connection we read
	// from is paired with because if we don't close it
	// it will leak unnecessarily
	std::shared_ptr<Pipe> m_SinkPipe;

	bool m_Closed;
	// The rate at which the pipe sends data
	u32 m_RateMS;
	u32 m_MaxSize;
};

}  // namespace thread
}  // namespace mcidle

#pragma once

#include <memory>
#include <string>
#include <boost/asio.hpp>

namespace mcidle {

class TCPSocket
{
public:
	TCPSocket() = default;
	TCPSocket(TCPSocket&);
	TCPSocket(std::string address, std::string port)
		: m_Address(address), m_Port(port), m_Service(new boost::asio::io_service) { }

	bool Connect();
	std::size_t Send(boost::asio::mutable_buffer&);
	std::size_t Recv(boost::asio::mutable_buffer&);
	// Block until bytes are read
	std::size_t Read(boost::asio::mutable_buffer&);
private:
	std::unique_ptr<boost::asio::io_service> m_Service;
	std::unique_ptr<boost::asio::ip::tcp::socket> m_Socket;
	std::string m_Address;
	std::string m_Port;
};

} // namespace mcidle

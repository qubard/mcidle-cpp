#pragma once

#include <memory>
#include <string>
#include <boost/asio.hpp>

#include <common/Typedef.hpp>

namespace mcidle {

using boost::asio::ip::tcp;

class TCPSocket
{
public:
	TCPSocket(s32 bindPort) : m_Service(new boost::asio::io_service), 
		m_Socket(std::make_unique<tcp::socket>(*m_Service)) {
		m_Acceptor = std::make_unique<tcp::acceptor>(*m_Service, 
			tcp::endpoint(tcp::v4(), bindPort));
	};
	TCPSocket(TCPSocket&);
	TCPSocket(std::string address, std::string port)
		: m_Address(address), m_Port(port), m_Service(new boost::asio::io_service),
		m_Socket(std::make_unique<tcp::socket>(*m_Service)) { }

	bool Connect();
	bool Bind();
	std::size_t Send(boost::asio::mutable_buffer&);
	std::size_t Recv(boost::asio::mutable_buffer&);
	// Block until bytes are read
	std::size_t Read(boost::asio::mutable_buffer&);
private:
	std::unique_ptr<boost::asio::io_service> m_Service;
	std::unique_ptr<tcp::socket> m_Socket;
	std::unique_ptr<tcp::acceptor> m_Acceptor;
	std::string m_Address;
	std::string m_Port;
};

} // namespace mcidle

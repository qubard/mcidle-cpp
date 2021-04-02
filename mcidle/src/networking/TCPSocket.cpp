#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <networking/TCPSocket.hpp>
#include <networking/types/VarInt.hpp>

namespace mcidle
{

TCPSocket::TCPSocket(s32 bindPort) :
	m_Service(new boost::asio::io_service),
	m_Socket(std::make_unique<tcp::socket>(*m_Service))
{
	m_Acceptor = std::make_unique<tcp::acceptor>(
		*m_Service,
		tcp::endpoint(
			boost::asio::ip::address::from_string("0.0.0.0"), bindPort));
}

TCPSocket::TCPSocket(TCPSocket & socket)
{
	m_Service = std::move(socket.m_Service);
	m_Socket = std::move(socket.m_Socket);
	m_Address = socket.m_Address;
	m_Port = socket.m_Port;
}

TCPSocket::TCPSocket(std::string address, std::string port) :
	m_Address(address),
	m_Service(new boost::asio::io_service),
	m_Port(port),
	m_Socket(std::make_unique<tcp::socket>(*m_Service))
{
}

bool TCPSocket::Connect()
{
	try
	{
		tcp::resolver resolver(*m_Service);
		auto host = gethostbyname(m_Address.data());
		printf("errno: %d\n", h_errno);
		printf("HOST:%d\n", host);
		std::cout << "Trying to resolve " << m_Address << ":" << m_Port << "\n";
		tcp::resolver::query query(
			m_Address, m_Port,
			boost::asio::ip::resolver_query_base::numeric_service);
		printf("Calling connect...\n");
		tcp::resolver::iterator iter = resolver.resolve(m_Address, m_Port);

		// Allocate the io_service separately since it's a dependency
		// shared_ptr so that we don't accidentally deallocate io_service
		// while the socket is still in use.

		boost::asio::connect(*m_Socket, iter);
	}
	catch (boost::system::system_error & e)
	{
		return false;
	}
	return true;
}

bool TCPSocket::Bind()
{
	try
	{
		// Why use async_accept when we have to run io_service in a separate
		// thread for it to be "asynchronous" anyway (io_service runs in the
		// current thread) Both solutions end up doing the same thing and
		// behaving exactly the same except one relies on io_service which is
		// probably better for multiple accept routines running
		// m_Acceptor->async_accept(*m_Socket, boost::bind(&TCPSocket::OnAccept,
		// this, boost::asio::placeholders::error));
		// m_Service->run();
		m_Acceptor->accept(*m_Socket);
	}
	catch (boost::system::system_error & e)
	{
		std::cout << "Exception: " << e.what() << "\n";
		return false;
	}

	return true;
}

std::size_t TCPSocket::Send(boost::asio::mutable_buffer & buffer)
{
	if (m_Socket.get() == NULL)
	{
		throw std::runtime_error("Trying to write on an empty socket");
	}

	try
	{
		return m_Socket->send(buffer);
	}
	catch (boost::system::system_error & e)
	{
		return -1;
	}
	return 0;
}

std::size_t TCPSocket::Recv(boost::asio::mutable_buffer & buffer)
{
	try
	{
		std::size_t n = m_Socket->receive(buffer);
		return n;
	}
	catch (boost::system::system_error & e)
	{
		return -1;
	}

	return 0;
}

std::size_t TCPSocket::Read(boost::asio::mutable_buffer & buffer)
{
	try
	{
		return boost::asio::read(*m_Socket, buffer);
	}
	catch (boost::system::system_error & e)
	{
		return -1;
	}

	return 0;
}

}  // namespace mcidle

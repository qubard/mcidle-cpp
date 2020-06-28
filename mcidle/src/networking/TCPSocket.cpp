#include <iostream>
#include <networking/TCPSocket.hpp>
#include <networking/types/VarInt.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

namespace mcidle {

TCPSocket::TCPSocket(TCPSocket& socket)
{
	m_Service = std::move(socket.m_Service);
	m_Socket = std::move(socket.m_Socket);
	m_Address = socket.m_Address;
	m_Port = socket.m_Port;
}

bool TCPSocket::Connect()
{
	try
	{
		tcp::resolver resolver(*m_Service);
		tcp::resolver::query query(m_Address, m_Port);
		tcp::resolver::iterator iter = resolver.resolve(query);

		// Allocate the io_service separately since it's a dependency
		// shared_ptr so that we don't accidentally deallocate io_service
		// while the socket is still in use.

		boost::asio::connect(*m_Socket, iter);
	}
	catch (boost::system::system_error & e)
	{
		std::cout << "Exception: " << e.what() << "\n";
		return false;
	}
	return true;
}

bool TCPSocket::Bind()
{
	try
	{
		// Why use async_accept when we have to run io_service in a separate thread
		// for it to be "asynchronous" anyway (io_service runs in the current thread)
		// Both solutions end up doing the same thing and behaving exactly the same
		// except one relies on io_service which is probably better for multiple
		// accept routines running
		//m_Acceptor->async_accept(*m_Socket, boost::bind(&TCPSocket::OnAccept, 
		// this, boost::asio::placeholders::error));
		// m_Service->run();
		m_Acceptor->accept(*m_Socket);

		std::cout << "Accepted client\n";
	}
	catch (boost::system::system_error & e)
	{
		std::cout << "Exception: " << e.what() << "\n";
		return false;
	}


	return true;
}

std::size_t TCPSocket::Send(boost::asio::mutable_buffer& buffer)
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
		std::cout << e.what() << "\n";
		return 0;
	}
	return 0;
}

std::size_t TCPSocket::Recv(boost::asio::mutable_buffer& buffer)
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

std::size_t TCPSocket::Read(boost::asio::mutable_buffer& buffer)
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


}

#pragma once

#include <netdb.h>

#include <boost/asio.hpp>
#include <common/Typedef.hpp>
#include <memory>
#include <string>

namespace mcidle {

using boost::asio::ip::tcp;

class TCPSocket
{
public:
    // Bind ctor
    TCPSocket(s32);
    TCPSocket(TCPSocket &);
    // Connect ctor
    TCPSocket(std::string, std::string);

    bool Connect();
    bool Bind();

    // To support better error handling these should
    // probably return signed types..
    std::size_t Send(boost::asio::mutable_buffer &);
    std::size_t Recv(boost::asio::mutable_buffer &);
    // Block until bytes are read
    std::size_t Read(boost::asio::mutable_buffer &);

private:
    std::unique_ptr<boost::asio::io_service> m_Service;
    std::unique_ptr<tcp::socket> m_Socket;
    std::unique_ptr<tcp::acceptor> m_Acceptor;
    std::string m_Address;
    std::string m_Port;
};

}  // namespace mcidle

#include <networking/connection/GameConnection.hpp>

namespace mcidle {

GameConnection::GameConnection(std::unique_ptr<TCPSocket> socket, 
        std::shared_ptr<mcidle::Protocol> protocol,
       std::size_t readSize)
    : Connection(std::move(socket), protocol, readSize)
{
}

bool GameConnection::Setup()
{
    return false;
}

} // ns mcidle

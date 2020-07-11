#pragma once

#include <networking/connection/GameConnection.hpp>

namespace mcidle {

// SConnection represents a connection
// to the target Minecraft server
// Receive from the server, send to the server
class SConnection : public Connection
{
public:
    SConnection(std::unique_ptr<TCPSocket>, 
            std::shared_ptr<mcidle::Protocol>, 
            std::size_t);

    bool Setup(mcidle::util::Yggdrasil&) override;
};

} // ns mcidle

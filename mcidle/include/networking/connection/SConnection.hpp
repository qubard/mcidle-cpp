#pragma once

#include <networking/connection/Connection.hpp>

namespace mcidle {

// SConnection represents a connection
// to the target Minecraft server
// Receive from the server, send to the server
class SConnection : public Connection
{
public:
    SConnection(std::unique_ptr<TCPSocket>, 
            std::shared_ptr<mcidle::Protocol>, 
            std::shared_ptr<mcidle::game::GameState>,
            std::size_t);

    void SetServerIP(std::string);
    void SetOnlineMode(bool);
    bool Setup(mcidle::util::Yggdrasil&) override;
private:
    std::string m_ServerIP;
    bool m_OnlineMode;
};

} // ns mcidle

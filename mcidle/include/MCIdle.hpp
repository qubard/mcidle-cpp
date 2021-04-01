#pragma once

#include <common/Typedef.hpp>
#include <networking/connection/SConnection.hpp>
#include <networking/protocol/Protocol.hpp>
#include <util/Yggdrasil.hpp>

#include <networking/thread/Pipe.hpp>

namespace mcidle {

class MCIdle
{
public:
    MCIdle(bool, std::string, std::string, std::shared_ptr<Protocol>, std::shared_ptr<Protocol>, util::Yggdrasil);

    std::shared_ptr<game::GameState> GameState();

    // Start MCIdle and return true if successful
    bool Start();

private:
    util::Yggdrasil m_Yggdrasil;
    // The protocol for packets sent to the client
    std::shared_ptr<Protocol> m_Protocol_CB;
    // The protocol for packets sent to the server
    std::shared_ptr<Protocol> m_Protocol_SB;
    std::shared_ptr<game::GameState> m_State;

    std::shared_ptr<thread::Pipe> m_Client;
    std::shared_ptr<thread::Pipe> m_Server;

    std::shared_ptr<SConnection> m_ServerConn;
    std::string m_ServerIP;
    std::string m_Port;
    bool m_Online;  // online mode
};

}  // namespace mcidle

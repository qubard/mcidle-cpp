#pragma once

#include <common/Typedef.hpp>
#include <networking/protocol/Protocol.hpp>

namespace mcidle {

class MCIdle 
{
public:
    MCIdle(bool, std::string);

    // Start MCIdle and return true if successful
    bool Start();
private:
    // The protocol for packets sent to the client
    std::shared_ptr<Protocol> m_ProtocolCB;
    // The protocol for packets sent to the server
    std:: shared_ptr<Protocol> m_ProtocolSB;
    std::shared_ptr<game::GameState> m_State;
    std::string m_ServerIp;
    bool m_Online;
};

} // ns mcidle

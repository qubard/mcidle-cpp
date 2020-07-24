#pragma once

#include <common/Typedef.hpp>
#include <networking/protocol/Protocol.hpp>
#include <util/Yggdrasil.hpp>

namespace mcidle {

class MCIdle 
{
public:
    MCIdle(bool, std::string, std::shared_ptr<Protocol>, std::shared_ptr<Protocol>);

    bool Login(std::string, std::string);
    // Start MCIdle and return true if successful
    bool Start();
private:
    util::Yggdrasil m_Yggdrasil;
    // The protocol for packets sent to the client
    std::shared_ptr<Protocol> m_Protocol_CB;
    // The protocol for packets sent to the server
    std:: shared_ptr<Protocol> m_Protocol_SB;
    std::shared_ptr<game::GameState> m_State;
    std::string m_ServerIp;
    bool m_Online;
};

} // ns mcidle

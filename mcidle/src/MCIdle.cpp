#include <MCIdle.hpp>

namespace mcidle
{

MCIdle::MCIdle(bool online, std::string ip, std::shared_ptr<Protocol> protocolCB, std::shared_ptr<Protocol> protocolSB) : m_Online(online), m_ServerIp(ip), m_Protocol_CB(protocolCB), m_Protocol_SB(protocolSB)
{
}

bool Login(std::string username, std::string password)
{

}

bool MCIdle::Start()
{
    // Start the serverbound connection
    return true;
}

} // ns mcidle

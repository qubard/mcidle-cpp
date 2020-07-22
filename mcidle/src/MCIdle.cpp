#include <MCIdle.hpp>

namespace mcidle
{

MCIdle::MCIdle(bool online, std::string ip) : m_Online(online), m_ServerIp(ip)
{
}

} // ns mcidle

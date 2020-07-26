#include <MCIdle.hpp>
#include <networking/TCPSocket.hpp>
#include <networking/Proxy.hpp>

#include <networking/thread/ProxyPipe.hpp>
#include <boost/thread.hpp>

namespace mcidle
{

MCIdle::MCIdle(bool online, std::string serverIP, std::shared_ptr<Protocol> protocolCB, std::shared_ptr<Protocol> protocolSB, util::Yggdrasil yggdrasil) : m_Online(online), m_ServerIP(serverIP), m_Protocol_CB(protocolCB), m_Protocol_SB(protocolSB), m_State(std::make_shared<game::GameState>()), m_Yggdrasil(yggdrasil)
{
}

std::shared_ptr<game::GameState> MCIdle::GameState()
{
    return m_State;
}

bool MCIdle::Start()
{
    // Start a new connection as a client to the server
    auto socket = std::make_unique<TCPSocket>(m_ServerIP, "25565");

    // Connect the socket to the server
    if (!socket->Connect()) return false;

    m_SConn = std::make_shared<SConnection>(std::move(socket), m_Protocol_CB, m_State, 4096);

    if (!m_SConn->Setup(m_Yggdrasil))
        return false;

    thread::ProxyPipe pipe;
    boost::thread th();

    // Run the proxy which handles packet forwarding
    Proxy proxy(m_SConn, m_State);
    proxy.Run();

    // Begin waiting for a client to connect

    return true;
}

} // ns mcidle

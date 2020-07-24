#include <networking/Proxy.hpp>
#include <networking/packet/Serialize.hpp>

namespace mcidle {

Proxy::Proxy(std::shared_ptr<Connection> source, 
        std::shared_ptr<mcidle::game::GameState> state) : 
    m_Source(source), 
    m_State(state),
    m_Dest(nullptr)
{
}

// A proxy reads from `source` and writes to `sink`
// after it is setup.
Proxy::Proxy(std::shared_ptr<Connection> source, 
        std::shared_ptr<Connection> destination, 
        std::shared_ptr<mcidle::game::GameState> state)
    : m_Source(source), m_State(state), m_Dest(destination)
{
}

void Proxy::Run()
{
    for (;;)
    {
        auto packet = m_Source->ReadPacket();

        // Check that the packet is valid
        if (packet != nullptr)
        {
            // Try to generate a protocol agnostic response for the packet
            auto response = packet->Response(m_Source->Protocol(), m_Source->Compression());

            if (response != nullptr)
            {
                m_Source->SendPacketFwd(*response);
            }

            // Use the packet to mutate the game state attached to the proxy
            // Since this can move the packet do it last
            if (m_State != nullptr)
            {
                m_StateLock.lock();
                packet->Mutate(*m_State);
                m_StateLock.unlock();               
            }
        } 
        // This only occurs when the socket is closed
        else
        {
            break;
        }
    }
}

}  // namespace mcidle

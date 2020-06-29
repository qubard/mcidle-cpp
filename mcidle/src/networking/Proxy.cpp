#include <networking/Proxy.hpp>
#include <networking/packet/Serialize.hpp>

namespace mcidle {

Proxy::Proxy(std::shared_ptr<Connection> source, std::shared_ptr<GameState> state) : m_Source(source), m_State(state)
{
}

// A proxy reads from `source` and writes to `sink`
// after it is setup.
Proxy::Proxy(std::shared_ptr<Connection> source, std::shared_ptr<Connection> sink, std::shared_ptr<GameState> state)
    : m_Source(source), m_Sink(sink), m_State(state)
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
            m_StateLock.lock();
            if (m_State != nullptr)
                packet->Mutate(*m_State);
            m_StateLock.unlock();

            // Try to generate a protocol agnostic response for the packet
            auto response = packet->Response(m_Source->Protocol(), m_Source->Compression());

            if (response != nullptr)
                m_Source->SendPacketSimple(*response);
        }
    }
}

}  // namespace mcidle
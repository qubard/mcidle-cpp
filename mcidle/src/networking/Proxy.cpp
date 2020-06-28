#include <networking/Proxy.hpp>
#include <networking/packet/Serialize.hpp>

namespace mcidle
{

Proxy::Proxy(std::shared_ptr<Connection> source): m_Source(source)
{
}

// A proxy reads from `source` and writes to `sink`
// after it is setup.
Proxy::Proxy(std::shared_ptr<Connection> source, 
        std::shared_ptr<Connection> sink): m_Source(source), m_Sink(sink) 
{
}

void Proxy::RegisterHandler(std::shared_ptr<PacketHandler> handler)
{
    m_Handlers.push_back(handler);
}

void Proxy::Run()
{
    for(;;)
    {
        auto packet = m_Source->ReadPacket();

        // Check that the packet is valid
        if (packet != nullptr)
        {
            m_StateLock.lock();
            packet->Mutate(*m_State);
            m_StateLock.unlock();

            // Try to generate a protocol agnostic response for the packet
            auto response = packet->Response(m_Source->Protocol(), m_Source->Compression());

            if (response != nullptr)
                m_Source->SendPacketSimple(*response);
        } 
    }
}

}

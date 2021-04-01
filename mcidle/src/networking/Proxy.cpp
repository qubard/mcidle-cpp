#include <networking/Proxy.hpp>
#include <networking/packet/Serialize.hpp>

namespace mcidle {

Proxy::Proxy(std::shared_ptr<Connection> source,
             std::shared_ptr<mcidle::game::GameState> state)
    : m_Source(source)
    , m_State(state)
    , m_Dest(nullptr)
{
}

// A proxy reads from a source connection and writes to a destination
// pipe after it is setup. It is also responsible for closing
// the source pipe if it disconnects
Proxy::Proxy(std::shared_ptr<Connection> source,
             std::shared_ptr<thread::Pipe> dest,
             std::shared_ptr<mcidle::game::GameState> state)
    : m_Source(source)
    , m_State(state)
    , m_Dest(dest)
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
            std::cout << "Received packet id:" << packet->Id() << "\n";
            // Try to generate a protocol agnostic response for the packet
            auto response =
                packet->Response(m_Source->Protocol(), m_Source->Compression());

            // Send the response directly to source
            if (response != nullptr)
            {
                if (m_Source != nullptr)
                    m_Source->SendPacketFwd(*response);
            }
            else
            {
                // Send the packet down the pipe to a connected sink
                m_Dest->Push(packet);
            }

            // Use the packet to mutate the game state attached to the proxy
            if (m_State != nullptr)
            {
                packet->Mutate(*m_State);
            }
        }
        // This only occurs when the socket is closed
        else
        {
            printf("Packet is null!\n");
            break;
        }
    }
}

}  // namespace mcidle

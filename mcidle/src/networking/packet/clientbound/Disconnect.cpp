#include <iostream>
#include <networking/packet/clientbound/Disconnect.hpp>

namespace mcidle {
namespace packet {
    namespace clientbound {

        Disconnect::Disconnect()
        {
        }

        Disconnect::Disconnect(std::string reason)
            : m_Reason(reason)
        {
        }

        std::shared_ptr<Packet> Disconnect::Response(Protocol &protocol,
                                                     s32 compression)
        {
            // Generate a serverbound keep alive using our id
            std::cout << "Disconnected: " << m_Reason << "\n";
            return nullptr;
        }

        std::string Disconnect::Reason() const
        {
            return m_Reason;
        }

        Packet &Disconnect::Serialize()
        {
            return *this;
        }

        void Disconnect::Deserialize(ByteBuffer &buf)
        {
            buf >> m_Reason;
        }

    }  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

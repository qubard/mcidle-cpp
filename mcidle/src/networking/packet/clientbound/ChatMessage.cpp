#include <iostream>
#include <networking/packet/clientbound/ChatMessage.hpp>

namespace mcidle {
namespace packet {
    namespace clientbound {

        ChatMessage::ChatMessage()
        {
        }

        ChatMessage::ChatMessage(std::string data, s8 pos)
            : m_JsonData(data)
            , m_Position(pos)
        {
        }

        std::shared_ptr<Packet> ChatMessage::Response(Protocol &protocol,
                                                      s32 compression)
        {
            // Generate a serverbound keep alive using our id
            std::cout << m_JsonData << "\n";
            return nullptr;
        }

        std::string ChatMessage::JsonData() const
        {
            return m_JsonData;
        }

        Packet &ChatMessage::Serialize()
        {
            return *this;
        }

        void ChatMessage::Deserialize(ByteBuffer &buf)
        {
            buf >> m_JsonData;
            buf >> m_Position;
        }

    }  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

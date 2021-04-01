#pragma once

#include <networking/packet/Packet.hpp>

namespace mcidle {
namespace packet {
    namespace clientbound {

        class ChatMessage : public Packet
        {
        public:
            ChatMessage();
            ChatMessage(std::string, s8);

            virtual std::shared_ptr<Packet> Response(Protocol &, s32) override;

            std::string JsonData() const;

            Packet &Serialize() override;
            void Deserialize(ByteBuffer &) override;

        private:
            std::string m_JsonData;
            s8 m_Position;
        };

    }  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

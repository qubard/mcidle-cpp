#pragma once

#include <networking/packet/Packet.hpp>

namespace mcidle {
namespace packet {
    namespace serverbound {

        class LoginStart : public Packet
        {
        public:
            LoginStart();
            LoginStart(std::string);

            Packet &Serialize() override;
            void Deserialize(ByteBuffer &) override;

            std::string Username() const;

        private:
            std::string m_Username;
        };

    }  // namespace serverbound
}  // namespace packet
}  // namespace mcidle

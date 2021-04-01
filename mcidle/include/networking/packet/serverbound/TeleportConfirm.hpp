#pragma once

#include <networking/game/GameState.hpp>
#include <networking/packet/Packet.hpp>

namespace mcidle {
namespace packet {
    namespace serverbound {

        class TeleportConfirm : public Packet
        {
        public:
            TeleportConfirm();
            TeleportConfirm(s32);

            Packet &Serialize() override;
            void Deserialize(ByteBuffer &) override;

            s32 Id() const;

        private:
            s64 m_Id;
        };

    }  // namespace serverbound
}  // namespace packet
}  // namespace mcidle

#pragma once

#include <networking/game/GameState.hpp>
#include <networking/packet/Packet.hpp>

namespace mcidle {
namespace packet {
    namespace clientbound {

        class SpawnPosition : public Packet
        {
        public:
            SpawnPosition();
            SpawnPosition(s64, s64, s64);

            void Mutate(mcidle::game::GameState &) override;

            Packet &Serialize() override;
            void Deserialize(ByteBuffer &) override;

        private:
            s64 m_X;
            s64 m_Y;
            s64 m_Z;
        };

    }  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

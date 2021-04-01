#pragma once

#include <networking/game/GameState.hpp>
#include <networking/packet/Packet.hpp>

namespace mcidle {
namespace packet {
    namespace clientbound {

        class PlayerPosition : public Packet
        {
        public:
            PlayerPosition();
            PlayerPosition(double, double, double, bool);

            void Mutate(mcidle::game::GameState &) override;

            Packet &Serialize() override;
            void Deserialize(ByteBuffer &) override;

        private:
            double m_X;
            double m_FeetY;
            double m_Z;
            bool m_OnGround;
        };

    }  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

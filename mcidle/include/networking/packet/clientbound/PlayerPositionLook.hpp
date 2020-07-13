#pragma once

#include <networking/game/GameState.hpp>
#include <networking/packet/Packet.hpp>

namespace mcidle {

namespace packet {
namespace clientbound {

class PlayerPositionLook : public Packet
{
public:
    PlayerPositionLook();
    PlayerPositionLook(double, double, double, float, float, u8, s32);

    void Mutate(mcidle::game::GameState &) override;

    Packet &Serialize() override;
    void Deserialize(ByteBuffer &) override;

private:
    double m_X;
    double m_Y;
    double m_Z;
    float m_Yaw;
    float m_Pitch;
    u8 m_Flags;
    s32 m_TeleportId;
};

}  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

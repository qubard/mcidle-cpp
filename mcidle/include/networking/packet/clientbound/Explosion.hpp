#pragma once

#include <networking/game/GameState.hpp>
#include <networking/packet/Packet.hpp>
#include <networking/types/VarInt.hpp>

namespace mcidle {
namespace packet {
namespace clientbound {

struct Record {
    s8 X;
    s8 Y;
    s8 Z;
};

class Explosion : public Packet
{
public:
    Explosion();

    void Mutate(mcidle::game::GameState &) override;

    Packet &Serialize() override;
    void Deserialize(ByteBuffer &) override;

private:
    float m_X;
    float m_Y;
    float m_Z;
    float m_Radius;
    s32 m_RecordCount;
    float m_MotionX;
    float m_MotionY;
    float m_MotionZ;
    std::vector<Record> m_Records;
};

}  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

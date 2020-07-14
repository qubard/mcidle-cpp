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
    SpawnPosition(s32, s32, s32);

    void Mutate(mcidle::game::GameState &) override;

    Packet &Serialize() override;
    void Deserialize(ByteBuffer &) override;

private:
    s32 m_X;
    s32 m_Y;
    s32 m_Z;
};

}  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

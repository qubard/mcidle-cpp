#pragma once

#include <networking/game/GameState.hpp>
#include <networking/packet/Packet.hpp>
#include <networking/types/VarInt.hpp>

namespace mcidle {

namespace packet {
namespace clientbound {

class UpdateHealth : public Packet
{
public:
    UpdateHealth();
    UpdateHealth(float, s32, float);

    void Mutate(GameState &) override;

    Packet &Serialize() override;
    void Deserialize(ByteBuffer &) override;

private:
    float m_Health;
    VarInt m_Food;
    float m_Saturation;
};

}  // namespace clientbound
}  // namespace packet

}  // namespace mcidle

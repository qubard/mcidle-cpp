#pragma once

#include <networking/game/GameState.hpp>
#include <networking/packet/Packet.hpp>
#include <networking/types/Metadata.hpp>

namespace mcidle {
namespace packet {
namespace clientbound {

class SetSlot : public Packet
{
public:
    SetSlot();
    SetSlot(u8, s16, mcidle::Slot);

    void Mutate(mcidle::game::GameState &) override;

    Packet &Serialize() override;
    void Deserialize(ByteBuffer &) override;
private:
    u8 m_WindowID;
    s16 m_SlotNum;
    mcidle::Slot m_Slot;
};

}  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

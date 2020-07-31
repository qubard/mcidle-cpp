#pragma once

#include <networking/packet/Packet.hpp>

namespace mcidle {
namespace packet {
namespace clientbound {

class BlockChange : public Packet
{
public:
    BlockChange();
    BlockChange(s32, s32, s32, s32);

    Packet &Serialize() override;
    void Deserialize(ByteBuffer &) override;

    void Mutate(mcidle::game::GameState &state) override;
private:
    s64 m_X;
    s64 m_Y;
    s64 m_Z;
    s32 m_BlockID;
};

} // ns clientbound
} // ns packet
} // ns mcidle

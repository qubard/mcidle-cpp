#pragma once

#include <networking/packet/Packet.hpp>
#include <networking/types/VarInt.hpp>

namespace mcidle {
namespace packet {
namespace clientbound {

class SetCompression : public Packet
{
public:
    SetCompression();
    SetCompression(s32);

    s32 Threshold() const;

    Packet &Serialize() override;
    void Deserialize(ByteBuffer &) override;

private:
	VarInt m_Compression;
};

} // ns clientbound
} // ns packet
} // ns mcidle

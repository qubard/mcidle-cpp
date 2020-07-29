#pragma once

#include <networking/packet/Packet.hpp>

namespace mcidle {
namespace packet {
namespace clientbound {

class BlockChange : public Packet
{
public:
    BlockChange();

    Packet &Serialize() override;
    void Deserialize(ByteBuffer &) override;

    void Mutate(mcidle::game::GameState &state) override;
private:
};

} // ns clientbound
} // ns packet
} // ns mcidle

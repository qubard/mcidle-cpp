#include <networking/protocol/Protocol_1_12_2.hpp>

namespace mcidle {

Protocol_1_12_2_CB::Protocol_1_12_2_CB(s32 versionNumber)
    : Protocol(clientboundMap_1_12_2, versionNumber, state::HANDSHAKE)
{
}

s32 Protocol_1_12_2_CB::PacketId(packet::serverbound::KeepAlive &)
{
    return 0x0B;
}

s32 Protocol_1_12_2_CB::PacketId(packet::clientbound::UpdateHealth &)
{
    return 0x41;
}

Protocol_1_12_2_SB::Protocol_1_12_2_SB(s32 versionNumber)
    : Protocol(serverboundMap_1_12_2, versionNumber, state::HANDSHAKE)
{
}

}  // namespace mcidle

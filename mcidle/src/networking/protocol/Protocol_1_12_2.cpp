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

s32 Protocol_1_12_2_SB::PacketId(packet::clientbound::UpdateHealth &)
{
    return 0x41;
}

s32 Protocol_1_12_2_SB::PacketId(packet::clientbound::JoinGame &) 
{
    return 0x23;
}

s32 Protocol_1_12_2_SB::PacketId(packet::clientbound::SpawnPosition &) 
{
    return 0x46;
}

s32 Protocol_1_12_2_SB::PacketId(packet::clientbound::PlayerPositionLook &) 
{
    return 0x2F;
}

s32 Protocol_1_12_2_SB::PacketId(packet::clientbound::KeepAlive &) 
{
    return 0x1F;
}

s32 Protocol_1_12_2_SB::PacketId(packet::clientbound::ChunkData &) 
{
    return 0x20;
}


Protocol_1_12_2_SB::Protocol_1_12_2_SB(s32 versionNumber)
    : Protocol(serverboundMap_1_12_2, versionNumber, state::HANDSHAKE)
{
}

}  // namespace mcidle

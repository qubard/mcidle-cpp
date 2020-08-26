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

s32 Protocol_1_12_2_CB::PacketId(packet::serverbound::Handshake &)
{
    return 0x00;
}

s32 Protocol_1_12_2_CB::PacketId(packet::serverbound::LoginStart &)
{
    return 0x00;
}

s32 Protocol_1_12_2_CB::PacketId(packet::serverbound::EncryptionResponse &)
{
    return 0x01;
}

s32 Protocol_1_12_2_SB::PacketId(packet::clientbound::UpdateHealth &)
{
    return 0x41;
}

s32 Protocol_1_12_2_SB::PacketId(packet::clientbound::SetCompression &)
{
    return 0x03;
}

s32 Protocol_1_12_2_SB::PacketId(packet::clientbound::JoinGame &) 
{
    return 0x23;
}

s32 Protocol_1_12_2_SB::PacketId(packet::clientbound::LoginSuccess &) 
{
    return 0x02;
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

s32 Protocol_1_12_2_SB::PacketId(packet::clientbound::EncryptionRequest &)
{
    return 0x01;
}

s32 Protocol_1_12_2_SB::PacketId(packet::clientbound::SetSlot &)
{
    return 0x16;
}

Protocol_1_12_2_SB::Protocol_1_12_2_SB(s32 versionNumber)
    : Protocol(serverboundMap_1_12_2, versionNumber, state::HANDSHAKE)
{
}

}  // namespace mcidle

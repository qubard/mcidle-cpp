#include <networking/protocol/Protocol.hpp>

namespace mcidle {

Protocol::Protocol(s32 versionNumber) : m_VersionNumber(versionNumber), m_State(state::LOGIN)
{
}

Protocol::Protocol(ProtocolMap inboundMap, s32 versionNumber, s32 state)
    : m_VersionNumber(versionNumber), m_State(state)
{
    m_InboundMap = inboundMap;
}

PacketMap &Protocol::InboundMap()
{
    return m_InboundMap[m_State];
}

s32 Protocol::VersionNumber()
{
    return m_VersionNumber;
}

void Protocol::SetState(s32 state)
{
    m_State = state;
}

s32 Protocol::PacketId(packet::serverbound::EncryptionResponse &)
{
    return 0x01;
}

s32 Protocol::PacketId(packet::serverbound::Handshake &)
{
    return 0x00;
}

s32 Protocol::PacketId(packet::serverbound::LoginStart &)
{
    return 0x00;
}

s32 Protocol::PacketId(packet::clientbound::EncryptionRequest &)
{
    return 0x01;
}

s32 Protocol::PacketId(packet::clientbound::SetCompression &)
{
    return 0x03;
}

s32 Protocol::PacketId(packet::serverbound::KeepAlive &)
{
    return 0x00;
}

s32 Protocol::PacketId(packet::clientbound::LoginSuccess &)
{
    return 0x02;
}

s32 Protocol::PacketId(packet::clientbound::UpdateHealth &)
{
    return 0x00;
}

s32 Protocol::PacketId(packet::clientbound::JoinGame &)
{
    return 0x00;
}

s32 Protocol::PacketId(packet::clientbound::SpawnPosition &)
{
    return 0x00;
}

s32 Protocol::PacketId(packet::clientbound::PlayerPositionLook &)
{
    return 0x00;
}

s32 Protocol::PacketId(packet::clientbound::ChunkData &)
{
    return 0x00;
}

s32 Protocol::PacketId(packet::clientbound::KeepAlive &)
{
    return 0x00;
}

}  // namespace mcidle

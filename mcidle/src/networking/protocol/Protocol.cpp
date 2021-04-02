#include <networking/protocol/Protocol.hpp>

namespace mcidle
{

Protocol::Protocol(s32 versionNumber) :
	m_VersionNumber(versionNumber), m_State(state::LOGIN)
{
}

Protocol::Protocol(ProtocolMap inboundMap, s32 versionNumber, s32 state) :
	m_VersionNumber(versionNumber), m_State(state)
{
	m_InboundMap = inboundMap;
}

PacketMap & Protocol::InboundMap() { return m_InboundMap[m_State]; }

s32 Protocol::VersionNumber() { return m_VersionNumber; }

void Protocol::SetState(s32 state) { m_State = state; }

s32 Protocol::PacketId(packet::serverbound::EncryptionResponse &)
{
	throw std::runtime_error("Unimplemented serverbound::EncryptionResponse!");
}

s32 Protocol::PacketId(packet::serverbound::Handshake &)
{
	throw std::runtime_error("Unimplemented serverbound::Handshake!");
}

s32 Protocol::PacketId(packet::serverbound::LoginStart &)
{
	throw std::runtime_error("Unimplemented serverbound::LoginStart!");
}

s32 Protocol::PacketId(packet::clientbound::EncryptionRequest &)
{
	throw std::runtime_error("Unimplemented clientbound::EncryptionRequest!");
}

s32 Protocol::PacketId(packet::clientbound::SetCompression &)
{
	throw std::runtime_error("Unimplemented clientbound::SetCompression!");
}

s32 Protocol::PacketId(packet::serverbound::KeepAlive &)
{
	throw std::runtime_error("Unimplemented serverbound::KeepAlive!");
}

s32 Protocol::PacketId(packet::clientbound::LoginSuccess &)
{
	throw std::runtime_error("Unimplemented clientbound::LoginSucces!");
}

s32 Protocol::PacketId(packet::clientbound::UpdateHealth &)
{
	throw std::runtime_error("Unimplemented clientbound::UpdateHealth!");
}

s32 Protocol::PacketId(packet::clientbound::JoinGame &)
{
	throw std::runtime_error("Unimplemented clientbound::JoinGame!");
}

s32 Protocol::PacketId(packet::clientbound::SpawnPosition &)
{
	throw std::runtime_error("Unimplemented clientbound::SpawnPosition!");
}

s32 Protocol::PacketId(packet::clientbound::PlayerPositionLook &)
{
	throw std::runtime_error("Unimplemented clientbound::PlayerPositionLook!");
}

s32 Protocol::PacketId(packet::clientbound::ChunkData &)
{
	throw std::runtime_error("Unimplemented clientbound::ChunkData!");
}

s32 Protocol::PacketId(packet::clientbound::KeepAlive &)
{
	throw std::runtime_error("Unimplemented clientbound::KeepAlive!");
}

s32 Protocol::PacketId(packet::clientbound::SetSlot &)
{
	throw std::runtime_error("Unimplemented clientbound::SetSlot!");
}

s32 Protocol::PacketId(packet::clientbound::SpawnMob &)
{
	throw std::runtime_error("Unimplemented clientbound::SpawnMob!");
}

s32 Protocol::PacketId(packet::clientbound::EntityLookRelMove &)
{
	throw std::runtime_error("Unimplemented clientbound::EntityLookRelMove!");
}

s32 Protocol::PacketId(packet::clientbound::EntityVelocity &)
{
	throw std::runtime_error("Unimplemented clientbound::EntityVelocity!");
}

s32 Protocol::PacketId(packet::clientbound::DestroyEntities &)
{
	throw std::runtime_error("Unimplemented clientbound::DestroyEntities!");
}

}  // namespace mcidle

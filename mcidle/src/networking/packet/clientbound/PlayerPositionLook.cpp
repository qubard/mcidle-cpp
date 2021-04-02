#include <networking/packet/clientbound/PlayerPositionLook.hpp>
#include <networking/types/VarInt.hpp>

namespace mcidle
{
namespace packet
{
namespace clientbound
{

PlayerPositionLook::PlayerPositionLook() {}

PlayerPositionLook::PlayerPositionLook(
	double x, double y, double z, float yaw, float pitch, u8 flags,
	s32 teleportId) :
	m_X(x),
	m_Y(y),
	m_Z(z),
	m_Yaw(yaw),
	m_Pitch(pitch),
	m_Flags(flags),
	m_TeleportId(teleportId)
{
}

void PlayerPositionLook::Mutate(mcidle::game::GameState & state)
{
	state.SetPosition(
		m_X, m_Y, m_Z, m_Flags & 0x01, m_Flags & 0x02, m_Flags & 0x04);
	printf(
		"<%.2f, %.2f, %.2f> %.2f, %.2f flags: %d\n", m_X, m_Y, m_Z, m_Yaw,
		m_Pitch, m_Flags);
	state.SetYaw(m_Yaw, m_Flags & 0x10);
	state.SetPitch(m_Pitch, m_Flags & 0x08);
}

Packet & PlayerPositionLook::Serialize()
{
	*m_FieldBuf << m_X;
	*m_FieldBuf << m_Y;
	*m_FieldBuf << m_Z;
	*m_FieldBuf << m_Yaw;
	*m_FieldBuf << m_Pitch;
	*m_FieldBuf << m_Flags;
	*m_FieldBuf << VarInt(m_TeleportId);
	return *this;
}

void PlayerPositionLook::Deserialize(ByteBuffer & buf)
{
	buf >> m_X;
	buf >> m_Y;
	buf >> m_Z;
	buf >> m_Yaw;
	buf >> m_Pitch;
	buf >> m_Flags;
	VarInt tmp;
	buf >> tmp;
	m_TeleportId = tmp.Value();
}

}  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

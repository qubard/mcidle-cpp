#include <networking/packet/clientbound/EntityTeleport.hpp>

namespace mcidle
{
namespace packet
{
namespace clientbound
{

EntityTeleport::EntityTeleport() {}

EntityTeleport::EntityTeleport(
	VarInt EntityID, double X, double Y, double Z, u8 Yaw, u8 Pitch,
	bool OnGround) :
	m_EntityID(EntityID),
	m_X(X),
	m_Y(Y),
	m_Z(Z),
	m_Yaw(Yaw),
	m_Pitch(Pitch),
	m_OnGround(OnGround)
{
}

void EntityTeleport::Mutate(mcidle::game::GameState & state)
{
	state.UpdateEntityPosition(
		m_EntityID.Value(), m_X, m_Y, m_Z, 0.0, 0.0, 0.0);
}

Packet & EntityTeleport::Serialize()
{
	*m_FieldBuf << m_EntityID;
	*m_FieldBuf << m_X;
	*m_FieldBuf << m_Y;
	*m_FieldBuf << m_Z;
	*m_FieldBuf << m_Yaw;
	*m_FieldBuf << m_Pitch;
	*m_FieldBuf << m_OnGround;
	return *this;
}

void EntityTeleport::Deserialize(ByteBuffer & buf)
{
	buf >> m_EntityID;
	buf >> m_X;
	buf >> m_Y;
	buf >> m_Z;
	buf >> m_Yaw;
	buf >> m_Pitch;
	buf >> m_OnGround;
}

}  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

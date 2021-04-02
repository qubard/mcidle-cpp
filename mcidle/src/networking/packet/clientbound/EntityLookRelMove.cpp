#include <networking/packet/clientbound/EntityLookRelMove.hpp>

namespace mcidle
{
namespace packet
{
namespace clientbound
{

EntityLookRelMove::EntityLookRelMove() {}

EntityLookRelMove::EntityLookRelMove(
	VarInt EntityID, s16 DeltaX, s16 DeltaY, s16 DeltaZ, u8 Yaw, u8 Pitch,
	bool OnGround) :
	m_EntityID(EntityID),
	m_DeltaX(DeltaX),
	m_DeltaY(DeltaY),
	m_DeltaZ(DeltaZ),
	m_Yaw(Yaw),
	m_Pitch(Pitch),
	m_OnGround(OnGround)
{
}

void EntityLookRelMove::Mutate(mcidle::game::GameState & state)
{
	state.TranslateEntity(m_EntityID.Value(), m_DeltaX, m_DeltaY, m_DeltaZ);
	printf(
		"EntityLOOKRelMove for %d: %d %d %d\n", m_EntityID.Value(), m_DeltaX,
		m_DeltaY, m_DeltaZ);
}

Packet & EntityLookRelMove::Serialize()
{
	*m_FieldBuf << m_EntityID;
	*m_FieldBuf << m_DeltaX;
	*m_FieldBuf << m_DeltaY;
	*m_FieldBuf << m_DeltaZ;
	*m_FieldBuf << m_Yaw;
	*m_FieldBuf << m_Pitch;
	*m_FieldBuf << m_OnGround;
	return *this;
}

void EntityLookRelMove::Deserialize(ByteBuffer & buf)
{
	buf >> m_EntityID;
	buf >> m_DeltaX;
	buf >> m_DeltaY;
	buf >> m_DeltaZ;
	buf >> m_Yaw;
	buf >> m_Pitch;
	buf >> m_OnGround;
}

}  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

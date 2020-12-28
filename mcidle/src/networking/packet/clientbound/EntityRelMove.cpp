#include <networking/packet/clientbound/EntityRelMove.hpp>

namespace mcidle {
namespace packet {
namespace clientbound {

EntityRelMove::EntityRelMove()
{
}

EntityRelMove::EntityRelMove(VarInt EntityID, s16 DeltaX, s16 DeltaY, s16 DeltaZ, bool OnGround)
    : m_EntityID(EntityID), m_DeltaX(DeltaX), m_DeltaY(DeltaY), m_DeltaZ(DeltaZ), m_OnGround(OnGround)
{
}

void EntityRelMove::Mutate(mcidle::game::GameState &state)
{
    state.TranslateEntity(m_EntityID.Value(), m_DeltaX, m_DeltaY, m_DeltaZ);
}

Packet& EntityRelMove::Serialize()
{
    *m_FieldBuf << m_EntityID;
    *m_FieldBuf << m_DeltaX;
    *m_FieldBuf << m_DeltaY;
    *m_FieldBuf << m_DeltaZ;
    *m_FieldBuf << m_OnGround;

    return *this;
}

void EntityRelMove::Deserialize(ByteBuffer &buf)
{
    buf >> m_EntityID;
    buf >> m_DeltaX;
    buf >> m_DeltaY;
    buf >> m_DeltaZ;
    buf >> m_OnGround;
}

}  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

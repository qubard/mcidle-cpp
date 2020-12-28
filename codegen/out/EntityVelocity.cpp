#include <networking/packet/clientbound/EntityVelocity.hpp>

namespace mcidle {
namespace packet {
namespace clientbound {

EntityVelocity::EntityVelocity()
{
}

EntityVelocity::EntityVelocity(VarInt EntityID, s16 VelX, s16 VelY, s16 VelZ)
    : m_EntityID(EntityID), m_VelX(VelX), m_VelY(VelY), m_VelZ(VelZ)
{
}

void EntityVelocity::Mutate(mcidle::game::GameState &state)
{
}

Packet& EntityVelocity::Serialize()
{
    *m_FieldBuf << m_EntityID;
    *m_FieldBuf << m_VelX;
    *m_FieldBuf << m_VelY;
    *m_FieldBuf << m_VelZ;
    return *this;
}

void EntityVelocity::Deserialize(ByteBuffer &buf)
{
    buf >> m_EntityID;
    buf >> m_VelX;
    buf >> m_VelY;
    buf >> m_VelZ;
}

}  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

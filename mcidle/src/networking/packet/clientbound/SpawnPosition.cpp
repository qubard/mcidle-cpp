#include <networking/game/GameState.hpp>
#include <networking/packet/clientbound/SpawnPosition.hpp>

#include <networking/types/Position.hpp>

namespace mcidle {
namespace packet {
namespace clientbound {

SpawnPosition::SpawnPosition()
{
}

SpawnPosition::SpawnPosition(s64 x, s64 y, s64 z) : m_X(x), m_Y(y), m_Z(z)
{
}

void SpawnPosition::Mutate(mcidle::game::GameState &state)
{
    printf("Spawn position: %d, %d, %d\n", m_X, m_Y, m_Z);
    state.SetSpawnPosition(m_X, m_Y, m_Z);
}

Packet& SpawnPosition::Serialize()
{
    Position loc { m_X, m_Y, m_Z };
    *m_FieldBuf << loc;
    return *this;
}

void SpawnPosition::Deserialize(ByteBuffer &buf)
{
    Position loc;
    buf >> loc;

    m_X = loc.X;
    m_Y = loc.Y;
    m_Z = loc.Z;
}

}  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

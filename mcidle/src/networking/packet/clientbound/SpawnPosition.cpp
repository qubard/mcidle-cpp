#include <networking/game/GameState.hpp>
#include <networking/packet/clientbound/SpawnPosition.hpp>

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
    s64 pos = ((m_X & 0x3FFFFFF) << 38) | ((m_Y & 0xFFF) << 26) | (m_Z & 0x3FFFFFF);
    *m_FieldBuf << pos;
    return *this;
}

void SpawnPosition::Deserialize(ByteBuffer &buf)
{
    // Eventually add a position type?
    s64 pos;
    buf >> pos;
    s32 x = pos >> 38;
    s32 y = (pos >> 26) & 0xFFF;
    s32 z = pos & 0x3FFFFFF;

    if (x >= 2 << 25) x -= 2 << 26;
    if (y >= 2 << 11) y -= 2 << 12;
    if (z >= 2 << 25) z -= 2 << 26;

    m_X = x;
    m_Y = y;
    m_Z = z;
}

}  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

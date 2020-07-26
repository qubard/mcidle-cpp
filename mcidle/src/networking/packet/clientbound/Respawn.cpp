#include <networking/game/GameState.hpp>
#include <networking/packet/clientbound/Respawn.hpp>

namespace mcidle {
namespace packet {
namespace clientbound {

Respawn::Respawn()
{
}

Respawn::Respawn(int dimension, u8 difficulty, u8 gamemode, std::string level)
    : m_Dimension(dimension), m_Difficulty(difficulty), m_Gamemode(gamemode), m_Level(level)
{
}

void Respawn::Mutate(mcidle::game::GameState &state)
{
    state.SetDimension(m_Dimension);
    state.SetGamemode(m_Gamemode);
    printf("Respawn received, dim: %d gamemode: %d\n", m_Dimension, m_Gamemode);
}

Packet &Respawn::Serialize()
{
    *m_FieldBuf << m_Dimension;
    *m_FieldBuf << m_Difficulty;
    *m_FieldBuf << m_Gamemode;
    *m_FieldBuf << m_Level;

    return *this;
}

void Respawn::Deserialize(ByteBuffer &buf)
{
    buf >> m_Dimension;
    buf >> m_Difficulty;
    buf >> m_Gamemode;
    buf >> m_Level;
}

}  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

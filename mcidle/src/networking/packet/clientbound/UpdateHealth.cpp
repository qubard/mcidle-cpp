#include <networking/game/GameState.hpp>
#include <networking/packet/clientbound/UpdateHealth.hpp>

namespace mcidle {
namespace packet {
namespace clientbound {

UpdateHealth::UpdateHealth()
{
}

UpdateHealth::UpdateHealth(float health, s32 food, float saturation)
    : m_Health(health), m_Food(VarInt(food)), m_Saturation(saturation)
{
}

void UpdateHealth::Mutate(mcidle::game::GameState &state)
{
    printf("Updated food stats %.2f %d %.2f\n", m_Health, m_Food.Value(), m_Saturation);
    state.SetFoodStats(m_Health, m_Food.Value(), m_Saturation);
}

Packet &UpdateHealth::Serialize()
{
    *m_FieldBuf << m_Health;
    *m_FieldBuf << m_Food;
    *m_FieldBuf << m_Saturation;

    return *this;
}

void UpdateHealth::Deserialize(ByteBuffer &buf)
{
    buf >> m_Health;
    buf >> m_Food;
    buf >> m_Saturation;
}

}  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

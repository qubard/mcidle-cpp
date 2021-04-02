#include <networking/game/GameState.hpp>
#include <networking/packet/clientbound/GameState.hpp>

namespace mcidle
{
namespace packet
{
namespace clientbound
{

GameState::GameState() {}

GameState::GameState(u8 reason, float value) : m_Reason(reason), m_Value(value)
{
}

void GameState::Mutate(mcidle::game::GameState & state)
{
	if (m_Reason == CHANGE_GAMEMODE)
	{
		state.SetGamemode(static_cast<u8>(m_Value));
	}
}

Packet & GameState::Serialize()
{
	*m_FieldBuf << m_Reason;
	*m_FieldBuf << m_Value;

	return *this;
}

void GameState::Deserialize(ByteBuffer & buf)
{
	buf >> m_Reason;
	buf >> m_Value;
}

}  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

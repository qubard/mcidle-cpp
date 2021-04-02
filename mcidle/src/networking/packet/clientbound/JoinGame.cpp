#include <networking/packet/clientbound/JoinGame.hpp>

namespace mcidle
{
namespace packet
{
namespace clientbound
{

JoinGame::JoinGame() {}

JoinGame::JoinGame(
	s32 entityId, u8 gamemode, s32 dimension, u8 difficulty, u8 maxPlayers,
	std::string levelType, bool debugInfo) :
	m_EntityId(entityId),
	m_Gamemode(gamemode),
	m_Dimension(dimension),
	m_Difficulty(difficulty),
	m_MaxPlayers(maxPlayers),
	m_LevelType(levelType),
	m_DebugInfo(debugInfo)
{
}

void JoinGame::Mutate(mcidle::game::GameState & state)
{
	printf(
		"Join Game received entity: %d gamemode: %d dim: %d diff: "
		"%d maxplayers: "
		"%d\n",
		m_EntityId, m_Gamemode, m_Dimension, m_Difficulty, m_MaxPlayers);
	state.SetDimension(m_Dimension);
	state.SetGamemode(m_Gamemode);	// bit 3 reserved for hardcore status
	state.SetPlayerId(m_EntityId);
	state.SetDifficulty(m_Difficulty);
	state.SetMaxPlayers(m_MaxPlayers);
	state.SetLevelType(m_LevelType);
	state.SetDebugInfo(m_DebugInfo);
}

Packet & JoinGame::Serialize()
{
	*m_FieldBuf << m_EntityId;
	*m_FieldBuf << m_Gamemode;
	*m_FieldBuf << m_Dimension;
	*m_FieldBuf << m_Difficulty;
	*m_FieldBuf << m_MaxPlayers;
	*m_FieldBuf << m_LevelType;
	*m_FieldBuf << m_DebugInfo;
	return *this;
}

void JoinGame::Deserialize(ByteBuffer & buf)
{
	buf >> m_EntityId;
	buf >> m_Gamemode;
	buf >> m_Dimension;
	buf >> m_Difficulty;
	buf >> m_MaxPlayers;
	buf >> m_LevelType;
	buf >> m_DebugInfo;
}

}  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

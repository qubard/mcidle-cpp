#include <networking/game/GameState.hpp>
#include <networking/packet/clientbound/ChunkData.hpp>

namespace mcidle {
namespace game {

GameState::GameState()
{
}

void GameState::SetThreshold(s32 threshold)
{
    m_Threshold = threshold;
}

void GameState::SetFoodStats(float health, s32 food, float saturation)
{
    m_Player.Food.Health = health;
    m_Player.Food.Hunger = food;
    m_Player.Food.Saturation = saturation;
}

void GameState::SetDimension(u8 dimension)
{
    m_Player.Dimension = dimension;
}

void GameState::SetGamemode(u8 gamemode)
{
    m_Player.Gamemode = gamemode;
}

s32 GameState::Dimension()
{
    return m_Player.Dimension;
}

void GameState::SetPosition(double x, double y, double z, bool relX, bool relY, bool relZ)
{
    m_Player.X = (relX ? m_Player.X : 0) + x;
    m_Player.Y = (relY ? m_Player.Y : 0) + y;
    m_Player.Z = (relZ ? m_Player.Z : 0) + z;
}

void GameState::SetYaw(float yaw, bool rel)
{
    m_Player.Yaw = (rel ? m_Player.Yaw : 0) + yaw;
}

void GameState::SetPitch(float pitch, bool rel)
{
    m_Player.Pitch = (rel ? m_Player.Pitch : 0) + pitch;
}

void GameState::SetAngles(float pitch, float yaw)
{
    // Always absolute angles
    SetYaw(yaw, false);
    SetPitch(pitch, false);
}

void GameState::LoadChunk(Chunk chunk) 
{
    m_LoadedChunks.push_back(chunk);
}

void GameState::SetDifficulty(u8 difficulty)
{
    m_Difficulty = difficulty;
}

void GameState::SetPlayerId(s32 id)
{
    m_Player.EntityId = id;
}

void GameState::SetMaxPlayers(u8 maxPlayers)
{
    m_MaxPlayers = maxPlayers;
}

void GameState::SetLevelType(std::string level)
{
    m_LevelType = level;
}

void GameState::SetDebugInfo(bool debug)
{
    m_DebugInfo = debug;
}

u8 GameState::Gamemode() const
{
    return m_Player.Gamemode;
}

u8 GameState::Dimension() const
{
    return m_Player.Dimension;
}

s32 GameState::PlayerId() const
{
    return m_Player.EntityId;
}

u8 GameState::Difficulty() const
{
    return m_Difficulty;
}

void GameState::SetSpawnPosition(s64 x, s64 y, s64 z)
{
    m_Player.SpawnX = x;
    m_Player.SpawnY = y;
    m_Player.SpawnZ = z;
}

s64 GameState::SpawnX() const
{
    return m_Player.SpawnX;
}

s64 GameState::SpawnY() const
{
    return m_Player.SpawnY;
}

s64 GameState::SpawnZ() const
{
    return m_Player.SpawnZ;
}

double GameState::PlayerX() const
{
    return m_Player.X;
}

double GameState::PlayerY() const
{
    return m_Player.Y;
}

double GameState::PlayerZ() const
{
    return m_Player.Z;
}

float GameState::PlayerYaw() const
{
    return m_Player.Yaw;
}

float GameState::PlayerPitch() const
{
    return m_Player.Pitch;
}

u8 GameState::MaxPlayers() const 
{
    return m_MaxPlayers;
}

std::string GameState::LevelType() const
{
    return m_LevelType;
}

bool GameState::DebugInfo() const
{
    return m_DebugInfo;
}

ChunkVec& GameState::LoadedChunks()
{
    return m_LoadedChunks;
}

} // namespace game
} // namespace mcidle

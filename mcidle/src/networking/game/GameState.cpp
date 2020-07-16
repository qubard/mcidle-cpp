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
    m_Player.foodStats.Health = health;
    m_Player.foodStats.Food = food;
    m_Player.foodStats.Saturation = saturation;
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

void GameState::LoadChunk(std::shared_ptr<mcidle::packet::clientbound::ChunkData> chunk) 
{
    m_LoadedChunks.push_back(chunk);
}

ChunkVec& GameState::LoadedChunks()
{
    return m_LoadedChunks;
}

} // namespace game
} // namespace mcidle

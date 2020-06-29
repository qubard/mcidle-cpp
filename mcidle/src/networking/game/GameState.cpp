#include <networking/game/GameState.hpp>

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

} // namespace game
} // namespace mcidle

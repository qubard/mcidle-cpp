#include <networking/game/GameState.hpp>

namespace mcidle
{

GameState::GameState()
{
}

void GameState::SetThreshold(s32 threshold)
{
    m_Threshold = threshold;
}

} // ns mcidle

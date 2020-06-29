#pragma once

#include <common/Typedef.hpp>

namespace mcidle {
namespace game {

struct FoodStats {
    float Health;
    s32 Food;
    float Saturation;
};

struct Player {
    double X;
    double Y;
    double Z;
    bool OnGround;
    u8 Gamemode;
    u8 Dimension;
    FoodStats foodStats;
};

class GameState
{
public:
    GameState();

    void SetThreshold(s32);
    void SetFoodStats(float, s32, float);
    void SetGamemode(u8);
    void SetDimension(u8);
private:
    s32 m_Threshold;
    Player m_Player;
};

} // namespace game
} // namespace mcidle

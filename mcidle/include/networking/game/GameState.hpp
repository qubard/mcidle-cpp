#pragma once

#include <common/Typedef.hpp>

namespace mcidle {

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
    FoodStats foodStats;
};

class GameState
{
public:
    GameState();

    void SetThreshold(s32);
    void SetFoodStats(float, s32, float);

private:
    s32 m_Threshold;
    Player m_Player;
};

}  // namespace mcidle

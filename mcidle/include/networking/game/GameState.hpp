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
    float Yaw;
    float Pitch;
    bool OnGround;
    u8 Gamemode;
    s32 Dimension;
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

    // Set the position with absolute or relative coordinates/angles
    void SetPosition(double, double, double, bool, bool, bool);
    void SetYaw(float, bool);
    void SetPitch(float, bool);
    void SetAngles(float, float);

    s32 Dimension();
private:
    s32 m_Threshold;
    Player m_Player;
};

} // namespace game
} // namespace mcidle

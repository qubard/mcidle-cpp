#pragma once

#include <common/Typedef.hpp>

namespace mcidle
{

struct Player
{
    float Health;
    double X;
    double Y;
    double Z;
    bool OnGround;
};

class GameState
{
public:
    GameState();

    void SetThreshold(s32);
private:
    s32 m_Threshold;
    Player m_Player;
};

} // ns mcidle

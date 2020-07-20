#pragma once

#include <common/Typedef.hpp>

namespace mcidle {

namespace packet {
namespace clientbound {
class ChunkData;
}
}

namespace game {

struct FoodStats {
    float Health;
    s32 Hunger;
    float Saturation;
};

struct Player {
    s32 EntityId;
    double X;
    double Y;
    double Z;
    s64 SpawnX;
    s64 SpawnY;
    s64 SpawnZ;
    float Yaw;
    float Pitch;
    bool OnGround;
    u8 Gamemode;
    s32 Dimension;
    FoodStats Food;
};

using Chunk = std::shared_ptr<mcidle::packet::clientbound::ChunkData>;
using ChunkVec = std::vector<Chunk>;

class GameState
{
public:
    GameState();

    void SetThreshold(s32);
    void SetFoodStats(float, s32, float);

    // JoinGame information
    void SetGamemode(u8);
    void SetDimension(u8);
    void SetPlayerId(s32);
    void SetDifficulty(u8);
    void SetMaxPlayers(u8);
    void SetLevelType(std::string);
    void SetDebugInfo(bool);
    
    u8 Gamemode() const;
    u8 Dimension() const;
    s32 PlayerId() const;
    u8 Difficulty() const;
    u8 MaxPlayers() const;
    std::string LevelType() const;
    bool DebugInfo() const;

    void SetSpawnPosition(s64, s64, s64);
    s64 SpawnX() const;
    s64 SpawnY() const;
    s64 SpawnZ() const;

    // Set the position with absolute or relative coordinates/angles
    void SetPosition(double, double, double, bool, bool, bool);
    void SetYaw(float, bool);
    void SetPitch(float, bool);
    void SetAngles(float, float);

    double PlayerX() const;
    double PlayerY() const;
    double PlayerZ() const;
    float PlayerYaw() const;
    float PlayerPitch() const;

    // Load a chunk into the `m_LoadedChunks` table
    void LoadChunk(Chunk);

    ChunkVec& LoadedChunks();
    s32 Dimension();
private:
    Player m_Player;
    u8 m_Difficulty;
    u8 m_MaxPlayers;
    bool m_DebugInfo;
    std::string m_LevelType;

    s32 m_Threshold;

    ChunkVec m_LoadedChunks;
};

} // namespace game
} // namespace mcidle

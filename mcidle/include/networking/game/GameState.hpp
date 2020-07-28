#pragma once

#include <common/Typedef.hpp>
#include <boost/thread.hpp>

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

class GameState
{
public:
    GameState();

    void SetThreshold(s32);
    void SetFoodStats(float, s32, float);

    // JoinGame information
    void SetGamemode(u8);
    void SetDimension(s32);
    void SetPlayerId(s32);
    void SetDifficulty(u8);
    void SetMaxPlayers(u8);
    void SetLevelType(std::string);
    void SetDebugInfo(bool);
    
    u8 Gamemode() const;
    s32 Dimension() const;
    s32 PlayerId() const;
    u8 Difficulty() const;
    u8 MaxPlayers() const;
    std::string LevelType() const;
    bool DebugInfo() const;
    s32 Threshold() const;

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
    void LoadChunk(std::shared_ptr<Chunk>);
    void UnloadChunk(s32, s32);

    game::ChunkMap& LoadedChunks();

private:
    mutable boost::mutex m_Mutex;

    Player m_Player;
    u8 m_Difficulty;
    u8 m_MaxPlayers;
    bool m_DebugInfo;
    std::string m_LevelType;

    s32 m_Threshold;

    ChunkMap m_LoadedChunks;
};

} // namespace game
} // namespace mcidle

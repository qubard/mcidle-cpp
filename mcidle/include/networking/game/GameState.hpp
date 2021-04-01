#pragma once

#include <boost/thread.hpp>
#include <common/Typedef.hpp>
#include <networking/types/Metadata.hpp>

namespace mcidle {

namespace packet {
    namespace clientbound {
        class ChunkData;
    }
}  // namespace packet

namespace game {

    struct FoodStats {
        float Health;
        s32 Hunger;
        float Saturation;
    };

    // Similar to metadata's EntityData
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

        void TickEntities();
        void LoadEntity(EntityData);
        void UnloadEntity(s32 &);
        void TranslateEntity(s32, s16, s16, s16);
        void UpdateEntityVelocity(s32, s16, s16, s16);
        void UpdateEntityPosition(s32, double, double, double, double, double,
                                  double);

        void Lock();
        void Unlock();

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
        // Set the block at x, y, z in world space to blockID
        void SetChunkBlock(s32, s32, s32, s32);
        std::shared_ptr<Chunk> GetChunk(s32, s32, s32);
        void SetInventorySlot(s16, Slot);
        void ClearInventorySlot(s16);

        // Return copies since we don't want to expose a direct
        // reference to them (not thread safe)
        std::unordered_map<s16, Slot> PlayerInventory();
        std::unordered_map<s32, EntityData> LoadedEntities();

        game::ChunkMap &LoadedChunks();

    private:
        void SetLightData(s32, s32, s32, u8);

        mutable boost::mutex m_Mutex;

        Player m_Player;
        u8 m_Difficulty;
        u8 m_MaxPlayers;
        bool m_DebugInfo;
        std::string m_LevelType;

        s32 m_Threshold;
        // Inventory maps from slot number to Slot type
        std::unordered_map<s16, Slot> m_PlayerInventory;

        // Loaded entities maps from entity id to the entity
        std::unordered_map<s32, EntityData> m_LoadedEntities;

        ChunkMap m_LoadedChunks;
    };

}  // namespace game
}  // namespace mcidle

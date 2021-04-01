#include <networking/game/GameState.hpp>
#include <networking/packet/clientbound/ChunkData.hpp>

namespace mcidle {
namespace game {

    GameState::GameState()
    {
    }

    void GameState::SetThreshold(s32 threshold)
    {
        boost::lock_guard<boost::mutex> guard(m_Mutex);
        m_Threshold = threshold;
    }

    void GameState::SetFoodStats(float health, s32 food, float saturation)
    {
        boost::lock_guard<boost::mutex> guard(m_Mutex);
        m_Player.Food.Health = health;
        m_Player.Food.Hunger = food;
        m_Player.Food.Saturation = saturation;
    }

    void GameState::SetDimension(s32 dimension)
    {
        boost::lock_guard<boost::mutex> guard(m_Mutex);
        m_Player.Dimension = dimension;
    }

    void GameState::SetGamemode(u8 gamemode)
    {
        boost::lock_guard<boost::mutex> guard(m_Mutex);
        m_Player.Gamemode = gamemode;
    }

    s32 GameState::Dimension() const
    {
        boost::lock_guard<boost::mutex> guard(m_Mutex);
        return m_Player.Dimension;
    }

    void GameState::SetPosition(double x, double y, double z, bool relX, bool relY, bool relZ)
    {
        boost::lock_guard<boost::mutex> guard(m_Mutex);
        m_Player.X = (relX ? m_Player.X : 0) + x;
        m_Player.Y = (relY ? m_Player.Y : 0) + y;
        m_Player.Z = (relZ ? m_Player.Z : 0) + z;
    }

    void GameState::SetYaw(float yaw, bool rel)
    {
        boost::lock_guard<boost::mutex> guard(m_Mutex);
        m_Player.Yaw = (rel ? m_Player.Yaw : 0) + yaw;
    }

    void GameState::SetPitch(float pitch, bool rel)
    {
        boost::lock_guard<boost::mutex> guard(m_Mutex);
        m_Player.Pitch = (rel ? m_Player.Pitch : 0) + pitch;
    }

    void GameState::SetAngles(float pitch, float yaw)
    {
        boost::lock_guard<boost::mutex> guard(m_Mutex);
        // Always absolute angles
        SetYaw(yaw, false);
        SetPitch(pitch, false);
    }

    void GameState::LoadChunk(std::shared_ptr<Chunk> chunk)
    {
        boost::lock_guard<boost::mutex> guard(m_Mutex);
        auto pos = CalcChunkPos(chunk->ChunkX, chunk->ChunkZ);
        // new chunks may be added so we have to OR them
        // then replace the sections manually
        if (m_LoadedChunks.find(pos) == m_LoadedChunks.end())
        {
            m_LoadedChunks[pos] = chunk;
        }
        else
        {
            auto chnk = m_LoadedChunks[pos];
            // Update (replace) sections
            for (s32 y = 0; y < game::SECTION_SIZE; y++)
            {
                if (chunk->PrimaryBitMask & (1 << y))
                {
                    // Update lighting information
                    (*chnk->Sections)[y] = std::move((*chunk->Sections)[y]);
                    (*chnk->LightMap)[y] = std::move((*chunk->LightMap)[y]);
                    (*chnk->Skylight)[y] = std::move((*chunk->Skylight)[y]);
                    // Make sure to update the mask to include the new or updated section
                    chnk->PrimaryBitMask |= 1 << y;
                }
            }
        }
    }

    void GameState::Lock()
    {
        m_Mutex.lock();
    }

    void GameState::Unlock()
    {
        m_Mutex.unlock();
    }

    void GameState::UpdateEntityPosition(s32 id, double x, double y, double z, double vx, double vy, double vz)
    {
        boost::lock_guard<boost::mutex> guard(m_Mutex);
        if (m_LoadedEntities.find(id) == m_LoadedEntities.end())
            return;

        m_LoadedEntities[id].X = x;
        m_LoadedEntities[id].Y = y;
        m_LoadedEntities[id].Z = z;

        m_LoadedEntities[id].MotionX = vx;
        m_LoadedEntities[id].MotionY = vy;
        m_LoadedEntities[id].MotionZ = vz;
    }

    void GameState::UpdateEntityVelocity(s32 id, s16 vx, s16 vy, s16 vz)
    {
        boost::lock_guard<boost::mutex> guard(m_Mutex);
        if (m_LoadedEntities.find(id) == m_LoadedEntities.end())
            return;

        m_LoadedEntities[id].MotionX = vx;
        m_LoadedEntities[id].MotionY = vy;
        m_LoadedEntities[id].MotionZ = vz;
    }

    void GameState::TickEntities()
    {
        boost::lock_guard<boost::mutex> guard(m_Mutex);

        for (auto &p : m_LoadedEntities)
        {
            auto &ent = p.second;
            double dx = (double)4096.0 * (double)(ent.MotionX / 8000.0);
            double dy = (double)4096.0 * (double)(ent.MotionY / 8000.0);
            double dz = (double)4096.0 * (double)(ent.MotionZ / 8000.0);

            ent.X += dx;
            ent.Y += dy;
            ent.Z += dz;

            //printf("Updated entity position id %d: %.2f, %.2f, %.2f\n", p.first, dx, dy, dz);
        }
    }

    void GameState::LoadEntity(EntityData entity)
    {
        boost::lock_guard<boost::mutex> guard(m_Mutex);
        // server coordinates
        double d = 4096.0;
        entity.X *= d;
        entity.Y *= d;
        entity.Z *= d;
        m_LoadedEntities[entity.Id.Value()] = entity;
    }

    void GameState::UnloadEntity(s32 &id)
    {
        boost::lock_guard<boost::mutex> guard(m_Mutex);
        m_LoadedEntities.erase(id);
    }

    void GameState::TranslateEntity(s32 id, s16 dx, s16 dy, s16 dz)
    {
        boost::lock_guard<boost::mutex> guard(m_Mutex);

        if (m_LoadedEntities.find(id) == m_LoadedEntities.end())
            return;
        // don't normalize since we can normalize later
        /*double d = 4096.0;
    double a = (double)((long)dx/d);
    double b = (double)((long)dy/d);
    double c = (double)((long)dz/d);*/

        /*if (dx+dy+dz == 0)
    {
        m_LoadedEntities[id].MotionX = 0;
        m_LoadedEntities[id].MotionY = 0;
        m_LoadedEntities[id].MotionZ = 0;
    }*/

        m_LoadedEntities[id].ServerX += static_cast<s64>(dx);
        m_LoadedEntities[id].ServerY += static_cast<s64>(dy);
        m_LoadedEntities[id].ServerZ += static_cast<s64>(dz);
        //printf("Translated %d by %.2f, %.2f, %.2f aka %d, %d, %d\n", id, a, b, c, dx, dy, dz);
    }

    s32 GameState::Threshold() const
    {
        boost::lock_guard<boost::mutex> guard(m_Mutex);
        return m_Threshold;
    }

    void GameState::SetDifficulty(u8 difficulty)
    {
        boost::lock_guard<boost::mutex> guard(m_Mutex);
        m_Difficulty = difficulty;
    }

    void GameState::SetPlayerId(s32 id)
    {
        boost::lock_guard<boost::mutex> guard(m_Mutex);
        m_Player.EntityId = id;
    }

    void GameState::SetMaxPlayers(u8 maxPlayers)
    {
        boost::lock_guard<boost::mutex> guard(m_Mutex);
        m_MaxPlayers = maxPlayers;
    }

    void GameState::SetLevelType(std::string level)
    {
        boost::lock_guard<boost::mutex> guard(m_Mutex);
        m_LevelType = level;
    }

    void GameState::SetDebugInfo(bool debug)
    {
        boost::lock_guard<boost::mutex> guard(m_Mutex);
        m_DebugInfo = debug;
    }

    u8 GameState::Gamemode() const
    {
        boost::lock_guard<boost::mutex> guard(m_Mutex);
        return m_Player.Gamemode;
    }

    s32 GameState::PlayerId() const
    {
        boost::lock_guard<boost::mutex> guard(m_Mutex);
        return m_Player.EntityId;
    }

    u8 GameState::Difficulty() const
    {
        boost::lock_guard<boost::mutex> guard(m_Mutex);
        return m_Difficulty;
    }

    void GameState::SetSpawnPosition(s64 x, s64 y, s64 z)
    {
        boost::lock_guard<boost::mutex> guard(m_Mutex);
        m_Player.SpawnX = x;
        m_Player.SpawnY = y;
        m_Player.SpawnZ = z;
    }

    s64 GameState::SpawnX() const
    {
        boost::lock_guard<boost::mutex> guard(m_Mutex);
        return m_Player.SpawnX;
    }

    s64 GameState::SpawnY() const
    {
        boost::lock_guard<boost::mutex> guard(m_Mutex);
        return m_Player.SpawnY;
    }

    s64 GameState::SpawnZ() const
    {
        boost::lock_guard<boost::mutex> guard(m_Mutex);
        return m_Player.SpawnZ;
    }

    double GameState::PlayerX() const
    {
        boost::lock_guard<boost::mutex> guard(m_Mutex);
        return m_Player.X;
    }

    double GameState::PlayerY() const
    {
        boost::lock_guard<boost::mutex> guard(m_Mutex);
        return m_Player.Y;
    }

    double GameState::PlayerZ() const
    {
        boost::lock_guard<boost::mutex> guard(m_Mutex);
        return m_Player.Z;
    }

    float GameState::PlayerYaw() const
    {
        boost::lock_guard<boost::mutex> guard(m_Mutex);
        return m_Player.Yaw;
    }

    float GameState::PlayerPitch() const
    {
        boost::lock_guard<boost::mutex> guard(m_Mutex);
        return m_Player.Pitch;
    }

    u8 GameState::MaxPlayers() const
    {
        boost::lock_guard<boost::mutex> guard(m_Mutex);
        return m_MaxPlayers;
    }

    std::string GameState::LevelType() const
    {
        boost::lock_guard<boost::mutex> guard(m_Mutex);
        return m_LevelType;
    }

    bool GameState::DebugInfo() const
    {
        boost::lock_guard<boost::mutex> guard(m_Mutex);
        return m_DebugInfo;
    }

    // Need a thread safe foreach
    game::ChunkMap &GameState::LoadedChunks()
    {
        boost::lock_guard<boost::mutex> guard(m_Mutex);
        return m_LoadedChunks;
    }

    void GameState::SetInventorySlot(s16 slotNum, Slot slot)
    {
        boost::lock_guard<boost::mutex> guard(m_Mutex);
        m_PlayerInventory[slotNum] = slot;
    }

    void GameState::ClearInventorySlot(s16 slotNum)
    {
        boost::lock_guard<boost::mutex> guard(m_Mutex);
        m_PlayerInventory.erase(slotNum);
    }

    void GameState::UnloadChunk(s32 x, s32 z)
    {
        boost::lock_guard<boost::mutex> guard(m_Mutex);
        m_LoadedChunks.erase(CalcChunkPos(x, z));
    }

    std::unordered_map<s16, Slot> GameState::PlayerInventory()
    {
        boost::lock_guard<boost::mutex> guard(m_Mutex);
        return m_PlayerInventory;
    }

    std::unordered_map<s32, EntityData> GameState::LoadedEntities()
    {
        boost::lock_guard<boost::mutex> guard(m_Mutex);
        return m_LoadedEntities;
    }

    void GameState::SetLightData(s32 x, s32 y, s32 z, u8 value)
    {
        // Lookup the chunk
        auto chunk = GetChunk(x, y, z);

        if (chunk == nullptr)
            return;

        s32 chunkY = y / game::SECTION_SIZE;  // Chunk Y from world Y

        // Convert to relative block num coordinates
        auto blockNum = game::ChunkPosToBlockNum(x & 0xF, y & 0xF, z & 0xF);
        if (chunk->Skylight != nullptr)
            (*chunk->Skylight)[chunkY][blockNum] = value;
    }

    std::shared_ptr<Chunk> GameState::GetChunk(s32 x, s32 y, s32 z)
    {
        // Convert x, z to chunk space
        s32 chunkX, chunkZ;
        chunkX = x / game::SECTION_SIZE;
        chunkZ = z / game::SECTION_SIZE;

        if (x < 0 && x % game::SECTION_SIZE != 0)
            chunkX--;
        if (z < 0 && z % game::SECTION_SIZE != 0)
            chunkZ--;

        auto pos = game::CalcChunkPos(chunkX, chunkZ);

        // Ignore unloaded chunks
        if (m_LoadedChunks.find(pos) == m_LoadedChunks.end())
            return nullptr;

        // Lookup the chunk
        auto chunk = m_LoadedChunks[pos];

        s32 chunkY = y / game::SECTION_SIZE;  // Chunk Y from world Y

        // Create a new section if it doesn't exist in the chunk
        if ((*chunk->Sections).find(chunkY) == (*chunk->Sections).end())
            CreateNewSection(chunk, chunkY);

        return chunk;
    }

    void GameState::SetChunkBlock(s32 x, s32 y, s32 z, s32 blockID)
    {
        boost::lock_guard<boost::mutex> guard(m_Mutex);

        // Lookup the chunk
        auto chunk = GetChunk(x, y, z);

        if (chunk == nullptr)
            return;

        bool deleted = blockID == 0;

        s32 chunkY = y / game::SECTION_SIZE;  // Chunk Y from world Y
        // Convert to relative block num coordinates
        auto blockNum = game::ChunkPosToBlockNum(x & 0xF, y & 0xF, z & 0xF);
        (*chunk->Sections)[chunkY][blockNum] = blockID;

        // If deleting the block, update the light info of the block below
        /*if (deleted)
    {
        s8 mask = 1 << 4 - 1;
        u8 light = LightAt(chunk->Skylight, x, y, z);
        SetLightAt(chunk->Skylight, x, y, z, 0xFF);
        SetLightAt(chunk->LightMap, x, y, z, 0xFF);
    }*/
    }

}  // namespace game
}  // namespace mcidle

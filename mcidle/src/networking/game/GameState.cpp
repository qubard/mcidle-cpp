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

void GameState::LoadEntity(EntityData entity)
{
    boost::lock_guard<boost::mutex> guard(m_Mutex);
    m_LoadedEntities.push_back(entity);
}

void GameState::UnloadEntity(s32& id)
{
    boost::lock_guard<boost::mutex> guard(m_Mutex);
    for (auto idx = 0; idx < m_LoadedEntities.size(); idx++)
    {
        auto entity = m_LoadedEntities[idx];
        if (entity.Id.Value() == id)
        {
            m_LoadedEntities.erase(m_LoadedEntities.begin() + idx);
        }
    }
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
game::ChunkMap& GameState::LoadedChunks()
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

std::vector<EntityData> GameState::LoadedEntities()
{
    boost::lock_guard<boost::mutex> guard(m_Mutex);
    return m_LoadedEntities;
}

void GameState::SetChunkBlock(s32 x, s32 y, s32 z, s32 blockID)
{
    boost::lock_guard<boost::mutex> guard(m_Mutex);

    // Convert x, z to chunk space
    s32 chunkX, chunkZ;
    chunkX = x / game::SECTION_SIZE;
    chunkZ = z / game::SECTION_SIZE;

    printf("%d %d %d %d\n", x, z, x % game::SECTION_SIZE, z % game::SECTION_SIZE);

    if (x < 0 && x % game::SECTION_SIZE != 0)
        chunkX--;
    if (z < 0 && z % game::SECTION_SIZE != 0)
        chunkZ--;

    auto pos = game::CalcChunkPos(chunkX, chunkZ);

    // Ignore unloaded chunks
    if (m_LoadedChunks.find(pos) == m_LoadedChunks.end()) 
    {
        printf("Trying to place block at unloaded chunk %d %d\n", chunkX, chunkZ);
        return;
    }

    // Lookup the chunk
    auto chunk = m_LoadedChunks[pos];

    s32 chunkY = y / game::SECTION_SIZE; // Chunk Y from world Y

    // Create a new section if it doesn't exist in the chunk
    if ((*chunk->Sections).find(chunkY) == (*chunk->Sections).end())
        CreateNewSection(chunk, chunkY);

    // Convert to relative block num coordinates
    auto blockNum = game::ChunkPosToBlockNum(x & 0xF, y & 0xF, z & 0xF);
    (*chunk->Sections)[chunkY][blockNum] = blockID;
}

} // namespace game
} // namespace mcidle

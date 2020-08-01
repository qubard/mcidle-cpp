#include <networking/game/GameState.hpp>
#include <networking/packet/clientbound/Explosion.hpp>

namespace mcidle {
namespace packet {
namespace clientbound {

Explosion::Explosion()
{
}

void Explosion::Mutate(mcidle::game::GameState &state)
{
    game::ChunkMap& m = state.LoadedChunks();

    for (Record& r: m_Records)
    {
        // Convert to world coordinates
        s32 x = m_X + r.X;
        s32 y = m_Y + r.Y;
        s32 z = m_Z + r.Z;

        s32 chunkX = x / game::SECTION_SIZE;
        s32 chunkZ = y / game::SECTION_SIZE;
        if (x < 0 && x % game::SECTION_SIZE != 0)
        {
            chunkX--;
        }
        if (z < 0 && z % game::SECTION_SIZE != 0)
        {
            chunkZ--;
        }
        auto pos = game::CalcChunkPos(chunkX, chunkZ);

        // Ignore unloaded chunks
        if (m.find(pos) == m.end()) return;

        // Lookup the chunk using its x, z pos
        auto chnk = m[pos];
        s32 posY = y / game::SECTION_SIZE; // Chunk Y from world Y

        // Create a new section if it doesn't exist in the chunk
        if ((*chnk->Sections).find(posY) == (*chnk->Sections).end()) 
        {
            CreateNewSection(chnk, posY);
        }

        // These coordinates are relative to the chunk (0-15)
        auto blockNum = game::ChunkPosToBlockNum(x & 0xF, y & 0xF, z & 0xF);
        (*chnk->Sections)[posY][blockNum] = 0;
        printf("Explosion!\n");
    }
}

Packet &Explosion::Serialize()
{
    return *this;
}

void Explosion::Deserialize(ByteBuffer &buf)
{
    buf >> m_X;
    buf >> m_Y;
    buf >> m_Z;
    buf >> m_Radius;
    buf >> m_RecordCount;

    for (s32 i = 0; i < m_RecordCount; i++)
    {
        s8 x, y, z;
        buf >> x;
        buf >> y;
        buf >> z;
        Record r { x, y, z };
        m_Records.push_back(r);
    }

    buf >> m_MotionX;
    buf >> m_MotionY;
    buf >> m_MotionZ;
}

}  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

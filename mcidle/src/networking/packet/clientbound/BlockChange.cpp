#include <networking/packet/clientbound/BlockChange.hpp>
#include <networking/types/Location.hpp>
#include <networking/types/VarInt.hpp>

#include <iostream>

namespace mcidle {
namespace packet {
namespace clientbound {

BlockChange::BlockChange()
{
}

BlockChange::BlockChange(s32 x, s32 y, s32 z, s32 blockID) : m_X(x), m_Y(y), m_Z(z), m_BlockID(blockID)
{
}

Packet &BlockChange::Serialize()
{
    Location loc { m_X, m_Y, m_Z };
    *m_FieldBuf << loc;
    *m_FieldBuf << VarInt(m_BlockID);

    return *this;
}

void BlockChange::Deserialize(ByteBuffer &buf)
{
    Location loc;
    buf >> loc;

    m_X = loc.X;
    m_Y = loc.Y;
    m_Z = loc.Z;

    VarInt tmp;
    buf >> tmp;

    m_BlockID = tmp.Value();
}

void BlockChange::Mutate(mcidle::game::GameState &state)
{
    printf("Received block change for %lld, %lld, %lld, id: %d, meta: %d\n", m_X, m_Y, m_Z, m_BlockID >> 4, m_BlockID & 0xF);
    game::ChunkMap& m = state.LoadedChunks();

    s32 chunkX = m_X / game::SECTION_SIZE;
    s32 chunkZ = m_Z / game::SECTION_SIZE;
    if (m_X < 0 && m_X % game::SECTION_SIZE != 0)
    {
        chunkX--;
    }
    if (m_Z < 0 && m_Z % game::SECTION_SIZE != 0)
    {
        chunkZ--;
    }
    auto pos = game::CalcChunkPos(chunkX, chunkZ);

    // Ignore unloaded chunks
    if (m.find(pos) == m.end()) return;

    // Lookup the chunk using its x, z pos
    auto chnk = m[pos];
    s32 posY = m_Y / 16; // Chunk Y from world Y

    if ((*chnk->Sections).find(posY) != (*chnk->Sections).end()) 
    {
        // These coordinates are relative to the chunk (0-15)
        auto blockNum = game::ChunkPosToBlockNum(m_X & 0xF, m_Y & 0xF, m_Z & 0xF);
        (*chnk->Sections)[posY][blockNum] = m_BlockID;
        printf("Set block..\n");
    }
    else {
        // Have to create a new section!!
        printf("BlockChange trying to update block in non-existent section!\n");
    }

}

} // ns clientbound
} // ns packet
} // ns mcidle

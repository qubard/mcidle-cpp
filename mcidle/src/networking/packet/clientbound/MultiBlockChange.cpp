#include <networking/packet/clientbound/MultiBlockChange.hpp>

namespace mcidle {
namespace packet {
namespace clientbound {

MultiBlockChange::MultiBlockChange()
{
}

Packet &MultiBlockChange::Serialize()
{
    *m_FieldBuf << m_ChunkX;
    *m_FieldBuf << m_ChunkZ;

    *m_FieldBuf << VarInt(m_RecordCount);

    for (Record& r: m_Records)
    {
        *m_FieldBuf << r.PosXZ;
        *m_FieldBuf << r.BlockY;
        *m_FieldBuf << r.BlockID;
    }
}

void MultiBlockChange::Deserialize(ByteBuffer & buf) 
{
    buf >> m_ChunkX;
    buf >> m_ChunkZ;
    VarInt tmp;
    buf >> tmp;

    m_RecordCount = tmp.Value();

    for (s32 i = 0; i < m_RecordCount; i++)
    {
        u8 posXZ;
        buf >> posXZ;

        u8 blockY;
        buf >> blockY;

        VarInt blockID;
        buf >> blockID;

        m_Records.push_back(Record{ posXZ, blockY, blockID });
    }
}

void MultiBlockChange::Mutate(mcidle::game::GameState &state)
{
    game::ChunkMap& m = state.LoadedChunks();
    auto pos = game::CalcChunkPos(m_ChunkX, m_ChunkZ);

    if (m.find(pos) == m.end()) 
        throw std::runtime_error("Chunk is not loaded for MultiBlockChange!");

    // Lookup the chunk using its x, z pos
    auto chnk = m[pos];
    for (Record& r: m_Records)
    {
        s32 posY = r.BlockY >> 4; // Chunk Y from world Y

        if ((*chnk->Sections).find(posY) == (*chnk->Sections).end()) 
            throw std::runtime_error("MultiBlockChange trying to update block in non-existent section!");

        s32 blockID = r.BlockID.Value();
        // These coordinates are relative to the chunk (0-15)
        s32 posZ = r.PosXZ & 0xF;
        s32 posX = (r.PosXZ >> 4) & 0xF;

        auto blockNum = game::ChunkPosToBlockNum(posX, r.BlockY % 16, posZ);
        (*chnk->Sections)[posY][blockNum] = 0;
    }
}

} // ns clientbound
} // ns packet
} // ns mcidle

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
    return *this;
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
    // TODO: Fix this (it's not thread safe)
    /*game::ChunkMap& m = state.LoadedChunks();
    auto pos = game::CalcChunkPos(m_ChunkX, m_ChunkZ);

    // Ignore unloaded chunks
    if (m.find(pos) == m.end()) return;

    // Lookup the chunk using its x, z pos
    auto chnk = m[pos];
    for (Record& r: m_Records)
    {
        s32 posY = r.BlockY / game::SECTION_SIZE; // Chunk Y from world Y

        // Create a new section if it doesn't exist in the chunk
        if ((*chnk->Sections).find(posY) == (*chnk->Sections).end()) 
            CreateNewSection(chnk, posY);

        s32 blockID = r.BlockID.Value();
        // These coordinates are relative to the chunk (0-15)
        s32 posZ = r.PosXZ & 0xF;
        s32 posX = (r.PosXZ >> 4) & 0xF;

        auto blockNum = game::ChunkPosToBlockNum(posX, r.BlockY & 0xF, posZ);
        (*chnk->Sections)[posY][blockNum] = blockID;
    }*/
    for (Record& r: m_Records)
    {
        state.SetChunkBlock((r.PosXZ & 0xF) * game::SECTION_SIZE, r.BlockY, ((r.PosXZ >> 4) & 0xF) * game::SECTION_SIZE, r.BlockID.Value());
    }
}

} // ns clientbound
} // ns packet
} // ns mcidle

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

            for (Record &r : m_Records)
            {
                *m_FieldBuf << r.PosXZ;
                *m_FieldBuf << r.BlockY;
                *m_FieldBuf << r.BlockID;
            }
            return *this;
        }

        void MultiBlockChange::Deserialize(ByteBuffer &buf)
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

                m_Records.push_back(Record{posXZ, blockY, blockID});
            }
        }

        void MultiBlockChange::Mutate(mcidle::game::GameState &state)
        {
            for (Record &r : m_Records)
            {
                state.SetChunkBlock((r.PosXZ & 0xF) * game::SECTION_SIZE,
                                    r.BlockY,
                                    ((r.PosXZ >> 4) & 0xF) * game::SECTION_SIZE,
                                    r.BlockID.Value());
            }
        }

    }  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

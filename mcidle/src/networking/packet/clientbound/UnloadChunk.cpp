#include <networking/packet/clientbound/UnloadChunk.hpp>

namespace mcidle {
namespace packet {
    namespace clientbound {

        UnloadChunk::UnloadChunk()
        {
        }

        UnloadChunk::UnloadChunk(s32 x, s32 z)
            : m_ChunkX(x)
            , m_ChunkZ(z)
        {
        }

        s32 UnloadChunk::ChunkX() const
        {
            return m_ChunkX;
        }

        s32 UnloadChunk::ChunkZ() const
        {
            return m_ChunkZ;
        }

        Packet &UnloadChunk::Serialize()
        {
            *m_FieldBuf << m_ChunkX;
            *m_FieldBuf << m_ChunkZ;
        }

        void UnloadChunk::Deserialize(ByteBuffer &buf)
        {
            buf >> m_ChunkX;
            buf >> m_ChunkZ;
        }

        void UnloadChunk::Mutate(mcidle::game::GameState &state)
        {
            state.UnloadChunk(m_ChunkX, m_ChunkZ);
        }

    }  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

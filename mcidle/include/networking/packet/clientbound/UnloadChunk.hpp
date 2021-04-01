#pragma once

#include <networking/packet/Packet.hpp>
#include <networking/types/VarInt.hpp>

namespace mcidle {
namespace packet {
    namespace clientbound {

        class UnloadChunk : public Packet
        {
        public:
            UnloadChunk();
            UnloadChunk(s32, s32);

            s32 ChunkX() const;
            s32 ChunkZ() const;

            Packet &Serialize() override;
            void Deserialize(ByteBuffer &) override;

            void Mutate(mcidle::game::GameState &state) override;

        private:
            s32 m_ChunkX;
            s32 m_ChunkZ;
        };

    }  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

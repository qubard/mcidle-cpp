#pragma once

#include <networking/packet/Packet.hpp>
#include <networking/types/VarInt.hpp>
#include <unordered_map>

namespace mcidle {
namespace packet {
    namespace clientbound {

        class ChunkData : public Packet
        {
        public:
            ChunkData();
            ChunkData(game::Chunk &);
            ChunkData(s32, s32, bool, s32);
            ChunkData(ChunkData &&);

            s32 ChunkX() const;
            s32 ChunkZ() const;

            std::shared_ptr<Packet> Response(Protocol &, s32) override;

            Packet &Serialize() override;
            void Deserialize(ByteBuffer &) override;

            void Mutate(mcidle::game::GameState &state) override;

        private:
            inline void ReadSection(ByteBuffer &, s32, s32, s32);
            inline void WriteSection(ByteBuffer &, s32, u8);

            // Map section height (y=0, y=15) to the section
            // In world coordinates section height * 16 is y-pos
            std::shared_ptr<std::unordered_map<s32, game::Section>> m_Sections;
            // Half a byte of lighting information per block
            std::shared_ptr<std::unordered_map<s32, std::vector<u8>>>
                m_LightMap;
            std::shared_ptr<std::unordered_map<s32, std::vector<u8>>>
                m_Skylight;

            std::shared_ptr<std::vector<u8>> m_Biomes;

            // Dimension is only present when we convert from game::Chunk
            s32 m_Dimension;
            s32 m_ChunkX;
            s32 m_ChunkZ;
            bool m_GroundUp;
            VarInt m_PrimaryBitMask;
        };

    }  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

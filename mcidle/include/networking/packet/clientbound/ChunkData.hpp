#pragma once

#include <networking/packet/Packet.hpp>
#include <networking/types/VarInt.hpp>

#include <unordered_map>

namespace mcidle {
namespace packet {
namespace clientbound {

const s32 SECTION_SIZE = 16;

// A section is 16*16*16 values in a 1D vector
// It's 16% slower to nest the vectors
using Section = std::vector<s32>;

class ChunkData : public Packet
{
public:
    ChunkData();
    ChunkData(s32, s32, bool, s32);

    Packet &Serialize() override;
    void Deserialize(ByteBuffer &) override;

    void Mutate(mcidle::game::GameState &state) override;

    std::unordered_map<s32, Section> &ChunkMap();

private:
	inline void ReadSection(ByteBuffer&, s32, s32, s32);
	void WriteSection(s32, u8);

	// Map section height (y=0, y=15) to the section
	// In world coordinates section height * 16 is y-pos
	std::unordered_map<s32, Section> m_ChunkMap;
	std::unordered_map<s32, std::vector<u8>> m_LightMap;

	std::vector<u8> m_Biomes;

	s32 m_ChunkX;
	s32 m_ChunkZ;
	bool m_GroundUp;
	VarInt m_PrimaryBitMask;
};

} // ns clientbound
} // ns packet
} // ns mcidle

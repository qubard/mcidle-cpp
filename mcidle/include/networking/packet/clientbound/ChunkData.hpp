#pragma once

#include <networking/packet/Packet.hpp>

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
	ChunkData()
	{
	}
	ChunkData(s32 chunkX, s32 chunkZ, bool groundUp, s32 primaryBitMask) : Packet(),
		m_ChunkX(chunkX), m_ChunkZ(chunkZ), m_GroundUp(groundUp), m_PrimaryBitMask(primaryBitMask)
	{
	}

	Packet& Serialize() override;
	void Deserialize(ByteBuffer&) override;

	std::unordered_map<s32, Section>& ChunkMap();
private:
	inline void ReadSection(ByteBuffer&, s32, s32, s32);

	// Map section height (y=0, y=15) to the section
	// In world coordinates section height * 16 is y-pos
	std::unordered_map<s32, Section> m_ChunkMap;

	s32 m_ChunkX;
	s32 m_ChunkZ;
	bool m_GroundUp;
	s32 m_PrimaryBitMask;
};

} // namespace clientbound
} // namespace packet
} // namespace mcidle

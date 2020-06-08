#pragma once

#include <networking/packet/Packet.hpp>

namespace mcidle {
namespace packet {
namespace clientbound {

const s32 SECTION_SIZE = 16;

class ChunkData : public Packet
{
public:
	ChunkData() : 
		m_Blocks(SECTION_SIZE, 
			std::vector<std::vector<s32>>(SECTION_SIZE, 
				std::vector<s32>(SECTION_SIZE))) {}
	ChunkData(s32 chunkX, s32 chunkZ, bool groundUp, s32 primaryBitMask) : Packet(),
		m_ChunkX(chunkX), m_ChunkZ(chunkZ), m_GroundUp(groundUp), m_PrimaryBitMask(primaryBitMask),
		m_Blocks(SECTION_SIZE, std::vector<std::vector<s32>>(SECTION_SIZE, std::vector<s32>(SECTION_SIZE))) {
	}
	Packet& Serialize() override;
	void Deserialize(ByteBuffer&) override;
private:
	inline void ReadSection(ByteBuffer&, s32, s32, s32);

	std::vector<std::vector<std::vector<s32>>> m_Blocks;

	s32 m_ChunkX;
	s32 m_ChunkZ;
	bool m_GroundUp;
	s32 m_PrimaryBitMask;
};

} // namespace clientbound
} // namespace packet
} // namespace mcidle

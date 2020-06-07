#pragma once

#include <networking/packet/Packet.hpp>

namespace mcidle {
namespace packet {
namespace clientbound {

class ChunkData : public Packet
{
public:
	ChunkData() {}
	ChunkData(s32 chunkX, s32 chunkZ, bool groundUp, s32 primaryBitMask) : Packet(),
	m_ChunkX(chunkX), m_ChunkZ(chunkZ), m_GroundUp(groundUp), m_PrimaryBitMask(primaryBitMask) {}
	Packet& Serialize() override;
	void Deserialize(ByteBuffer&) override;
private:
	inline void ReadSection(ByteBuffer&, s32, s32, s32);

	s32 m_ChunkX;
	s32 m_ChunkZ;
	bool m_GroundUp;
	s32 m_PrimaryBitMask;
};

} // namespace clientbound
} // namespace packet
} // namespace mcidle

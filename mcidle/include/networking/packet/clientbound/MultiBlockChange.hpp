#pragma once

#include <networking/packet/Packet.hpp>
#include <networking/types/VarInt.hpp>

namespace mcidle
{

struct Record
{
	// 4 high bits X, 4 low bits Z (encode coordinates relative to chunk)
	u8 PosXZ;
	// Y coordinate of the block in world space (0-255)
	u8 BlockY;
	// Block ID as given in the global palette
	// type = id >> 4, meta = id & 15
	VarInt BlockID;
};

namespace packet
{
namespace clientbound
{

class MultiBlockChange : public Packet
{
  public:
	MultiBlockChange();

	Packet & Serialize() override;
	void Deserialize(ByteBuffer &) override;

	void Mutate(mcidle::game::GameState & state) override;

  private:
	s32 m_ChunkX;
	s32 m_ChunkZ;
	s32 m_RecordCount;
	std::vector<Record> m_Records;
};

}  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

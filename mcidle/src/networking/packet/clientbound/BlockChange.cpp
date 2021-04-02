#include <iostream>
#include <networking/packet/clientbound/BlockChange.hpp>
#include <networking/types/Position.hpp>
#include <networking/types/VarInt.hpp>

namespace mcidle
{
namespace packet
{
namespace clientbound
{

BlockChange::BlockChange() {}

BlockChange::BlockChange(s32 x, s32 y, s32 z, s32 blockID) :
	m_X(x), m_Y(y), m_Z(z), m_BlockID(blockID)
{
}

Packet & BlockChange::Serialize()
{
	Position loc{m_X, m_Y, m_Z};
	*m_FieldBuf << loc;
	*m_FieldBuf << VarInt(m_BlockID);

	return *this;
}

void BlockChange::Deserialize(ByteBuffer & buf)
{
	Position loc;
	buf >> loc;

	m_X = loc.X;
	m_Y = loc.Y;
	m_Z = loc.Z;

	VarInt tmp;
	buf >> tmp;

	m_BlockID = tmp.Value();

	printf("Block change at %d, %d, %d to %d\n", m_X, m_Y, m_Z, m_BlockID);
}

void BlockChange::Mutate(mcidle::game::GameState & state)
{
	state.SetChunkBlock(m_X, m_Y, m_Z, m_BlockID);
}

}  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

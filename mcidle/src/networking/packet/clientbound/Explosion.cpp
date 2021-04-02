#include <networking/game/GameState.hpp>
#include <networking/packet/clientbound/Explosion.hpp>

namespace mcidle
{
namespace packet
{
namespace clientbound
{

Explosion::Explosion() {}

void Explosion::Mutate(mcidle::game::GameState & state)
{
	for (Record & r : m_Records)
	{
		s32 x = m_X + r.X;
		s32 y = m_Y + r.Y;
		s32 z = m_Z + r.Z;

		state.SetChunkBlock(x, y, z, 0);
	}
}

Packet & Explosion::Serialize() { return *this; }

void Explosion::Deserialize(ByteBuffer & buf)
{
	buf >> m_X;
	buf >> m_Y;
	buf >> m_Z;
	buf >> m_Radius;
	buf >> m_RecordCount;

	for (s32 i = 0; i < m_RecordCount; i++)
	{
		s8 x, y, z;
		buf >> x;
		buf >> y;
		buf >> z;
		Record r{x, y, z};
		m_Records.push_back(r);
	}

	buf >> m_MotionX;
	buf >> m_MotionY;
	buf >> m_MotionZ;
}

}  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

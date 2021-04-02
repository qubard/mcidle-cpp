#include <networking/packet/clientbound/SetCompression.hpp>

namespace mcidle
{
namespace packet
{
namespace clientbound
{

SetCompression::SetCompression() : Packet() {}

SetCompression::SetCompression(s32 threshold) :
	Packet(), m_Compression(threshold)
{
}

void SetCompression::Mutate(game::GameState & state)
{
	state.SetThreshold(m_Compression.Value());
}

Packet & SetCompression::Serialize()
{
	*m_FieldBuf << m_Compression;
	return *this;
}

void SetCompression::Deserialize(ByteBuffer & buf) { buf >> m_Compression; }

s32 SetCompression::Threshold() const { return m_Compression.Value(); }

}  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

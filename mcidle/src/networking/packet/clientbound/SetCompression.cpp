#include <networking/packet/clientbound/SetCompression.hpp>

namespace mcidle {
namespace packet {
namespace clientbound {

Packet& SetCompression::Serialize()
{
	*m_FieldBuf << m_Compression;
	return *this;
}

void SetCompression::Deserialize(ByteBuffer& buf)
{
	buf >> m_Compression;
}

s32 SetCompression::Threshold() const
{
	return m_Compression.Value();
}

} // ns clientbound
} // ns packet
} // ns mcidle


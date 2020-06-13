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

} // ns clientbound
} // ns packet
} // ns mcidle


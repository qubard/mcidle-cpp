#include <networking/packet/serverbound/KeepAlive.hpp>

namespace mcidle {
namespace packet {
namespace serverbound {

Packet& KeepAlive::Serialize()
{
	*m_FieldBuf << m_Id;
	return *this;
}

void KeepAlive::Deserialize(ByteBuffer& buf)
{
	buf >> m_Id;
}

} // ns serverbound
} // ns packet
} // ns mcidle


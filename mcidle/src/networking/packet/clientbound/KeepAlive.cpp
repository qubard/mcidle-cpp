#include <networking/packet/clientbound/KeepAlive.hpp>

namespace mcidle {
namespace packet {
namespace clientbound {

Packet& KeepAlive::Serialize()
{
	*m_FieldBuf << m_Id;
	return *this;
}

void KeepAlive::Deserialize(ByteBuffer& buf)
{
	buf >> m_Id;
}

s64 KeepAlive::KeepAliveId() const
{
	return m_Id;
}

} // ns clientbound
} // ns packet
} // ns mcidle


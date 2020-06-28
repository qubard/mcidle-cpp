#include <networking/packet/serverbound/KeepAlive.hpp>

namespace mcidle {
namespace packet {
namespace serverbound {

KeepAlive::KeepAlive() : Packet()
{
}

KeepAlive::KeepAlive(s64 id) : Packet(), m_Id(id)
{
}

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


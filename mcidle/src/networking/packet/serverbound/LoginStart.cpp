#include <networking/packet/serverbound/LoginStart.hpp>

namespace mcidle {
namespace packet {
namespace serverbound {

Packet& LoginStart::Serialize()
{
	*m_FieldBuf << m_Username;
	return *this;
}

void LoginStart::Deserialize(ByteBuffer& buf)
{
	buf >> m_Username;
}

}
}
}



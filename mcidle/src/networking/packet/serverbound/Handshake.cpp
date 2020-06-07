#include <networking/packet/serverbound/Handshake.hpp>
#include <networking/types/VarInt.hpp>

namespace mcidle {
namespace packet {
namespace serverbound {

Packet& Handshake::Serialize() 
{
	*m_FieldBuf << m_ProtocolVersion;
	*m_FieldBuf << m_ServerAddress;
	*m_FieldBuf << m_ServerPort;
	*m_FieldBuf << m_NextState;
	return *this;
}

void Handshake::Deserialize(ByteBuffer& buf) 
{
	buf >> m_ProtocolVersion;
	buf >> m_ServerAddress;
	buf >> m_ServerPort;
	buf >> m_NextState;
}

}
}
}



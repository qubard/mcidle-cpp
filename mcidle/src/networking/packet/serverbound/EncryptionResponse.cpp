#include <networking/packet/serverbound/EncryptionResponse.hpp>

namespace mcidle {
namespace packet {
namespace serverbound {

Packet& EncryptionResponse::Serialize()
{
	*m_FieldBuf << m_Secret;
	*m_FieldBuf << m_Token;
	return *this;
}

void EncryptionResponse::Deserialize(ByteBuffer& buf)
{
	buf >> m_Secret;
	buf >> m_Token;
}

}
}
}



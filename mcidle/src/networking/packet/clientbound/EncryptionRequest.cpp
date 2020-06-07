#include <networking/packet/clientbound/EncryptionRequest.hpp>

namespace mcidle {
namespace packet {
namespace clientbound {

Packet& EncryptionRequest::Serialize()
{
	*m_FieldBuf << m_ServerId;
	*m_FieldBuf << m_PubKey;
	*m_FieldBuf << m_Token;
	return *this;
}

void EncryptionRequest::Deserialize(ByteBuffer& buf)
{
	buf >> m_ServerId;
	buf >> m_PubKey;
	buf >> m_Token;
}

}
}
}



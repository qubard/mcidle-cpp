#include <networking/packet/clientbound/LoginSuccess.hpp>

namespace mcidle
{
namespace packet
{
namespace clientbound
{

Packet & LoginSuccess::Serialize()
{
	*m_FieldBuf << m_UUID;
	*m_FieldBuf << m_Username;
	return *this;
}

void LoginSuccess::Deserialize(ByteBuffer & buf)
{
	buf >> m_UUID;
	buf >> m_Username;
}

}  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

#include <networking/packet/serverbound/LoginStart.hpp>

namespace mcidle
{
namespace packet
{
namespace serverbound
{

LoginStart::LoginStart() : Packet() {}

LoginStart::LoginStart(std::string username) : Packet(), m_Username(username) {}

Packet & LoginStart::Serialize()
{
	*m_FieldBuf << m_Username;
	return *this;
}

void LoginStart::Deserialize(ByteBuffer & buf) { buf >> m_Username; }

std::string LoginStart::Username() const { return m_Username; }

}  // namespace serverbound
}  // namespace packet
}  // namespace mcidle

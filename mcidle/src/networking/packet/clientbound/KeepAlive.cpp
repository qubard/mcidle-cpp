#include <networking/packet/Serialize.hpp>
#include <networking/packet/clientbound/KeepAlive.hpp>
#include <networking/packet/serverbound/KeepAlive.hpp>

namespace mcidle {
namespace packet {
namespace clientbound {

KeepAlive::KeepAlive()
{
}

KeepAlive::KeepAlive(s64 id) : m_Id(id)
{
}

std::shared_ptr<Packet> KeepAlive::Response(Protocol &protocol, s32 compression)
{
    // Generate a serverbound keep alive using our id
    printf("Responded to KeepAlive\n");
    return CreatePacket<packet::serverbound::KeepAlive>(protocol, compression, m_Id);
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

s64 KeepAlive::KeepAliveId() const
{
	return m_Id;
}

} // ns clientbound
} // ns packet
} // ns mcidle


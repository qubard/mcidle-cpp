#include <networking/packet/serverbound/TeleportConfirm.hpp>
#include <networking/types/VarInt.hpp>

namespace mcidle {
namespace packet {
namespace serverbound {

TeleportConfirm::TeleportConfirm() : Packet()
{
}

TeleportConfirm::TeleportConfirm(s32 id) : Packet(), m_Id(id)
{
}

Packet& TeleportConfirm::Serialize()
{
	*m_FieldBuf << VarInt(m_Id);
	return *this;
}

void TeleportConfirm::Deserialize(ByteBuffer& buf)
{
    VarInt tmp;
	buf >> tmp;
    m_Id = tmp.Value();
}

s32 TeleportConfirm::Id() const
{
    return m_Id;
}

} // ns serverbound
} // ns packet
} // ns mcidle


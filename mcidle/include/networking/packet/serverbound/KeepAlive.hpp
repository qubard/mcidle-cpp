#pragma once

#include <networking/packet/Packet.hpp>

namespace mcidle {
namespace packet {
namespace serverbound {

class KeepAlive : public Packet
{
public:
	KeepAlive() : Packet()
	{
	}
	KeepAlive(s64 id) : Packet(), m_Id(id) {}

	Packet& Serialize() override;
	void Deserialize(ByteBuffer&) override;
private:
	s64 m_Id;
};

} // ns serverbound
} // ns packet
} // ns mcidle

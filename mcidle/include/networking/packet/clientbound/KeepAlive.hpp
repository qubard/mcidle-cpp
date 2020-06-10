#pragma once

#include <networking/packet/Packet.hpp>

namespace mcidle {
namespace packet {
namespace clientbound {

class KeepAlive : public Packet
{
public:
	KeepAlive() : Packet()
	{
	}
	KeepAlive(s64 id) : Packet(), m_Id(id) {}

	s64 KeepAliveId() const;

	Packet& Serialize() override;
	void Deserialize(ByteBuffer&) override;
private:
	s64 m_Id;
};

} // ns clientbound
} // ns packet
} // ns mcidle

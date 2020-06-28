#pragma once

#include <networking/packet/Packet.hpp>

namespace mcidle {
namespace packet {
namespace clientbound {

class KeepAlive : public Packet
{
public:
	KeepAlive();
	KeepAlive(s64);

	s64 KeepAliveId() const;

    std::shared_ptr<Packet> Response(Protocol&, s32) override;

	Packet& Serialize() override;
	void Deserialize(ByteBuffer&) override;
private:
	s64 m_Id;
};

} // ns clientbound
} // ns packet
} // ns mcidle

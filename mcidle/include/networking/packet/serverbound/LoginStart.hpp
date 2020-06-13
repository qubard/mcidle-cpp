#pragma once

#include <networking/packet/Packet.hpp>

namespace mcidle {
namespace packet {
namespace serverbound {

class LoginStart : public Packet 
{
public:
	LoginStart() : Packet() {}
	LoginStart(std::string username) : Packet(), m_Username(username) {}

	Packet& Serialize() override;
	void Deserialize(ByteBuffer&) override;

	std::string Username() const;
private:
	std::string m_Username;
};

} // namespace serverbound
} // namespace packet
} // namespace mcidle

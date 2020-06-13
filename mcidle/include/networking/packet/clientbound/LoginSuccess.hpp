#pragma once

#include <networking/packet/Packet.hpp>

namespace mcidle {
namespace packet {
namespace clientbound {

class LoginSuccess : public Packet
{
public:
	LoginSuccess() : Packet() {}
	LoginSuccess(std::string uuid, std::string username) : m_UUID(uuid), m_Username(username)
	{}

	Packet& Serialize() override;
	void Deserialize(ByteBuffer&) override;
private:
	std::string m_Username;
	std::string m_UUID;
};

} // namespace clientbound
} // namespace packet
} // namespace mcidle

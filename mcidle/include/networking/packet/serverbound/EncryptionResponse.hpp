#pragma once

#include <networking/packet/Packet.hpp>

namespace mcidle {
namespace packet {
namespace serverbound {

class EncryptionResponse : public Packet
{
public:
	EncryptionResponse() : Packet() {}
	EncryptionResponse(std::string& secret, std::string& token) :
		Packet(), m_Secret(std::move(secret)), m_Token(std::move(token)) {}

	std::string& Secret();
	std::string& Token();

	Packet& Serialize() override;
	void Deserialize(ByteBuffer&) override;
private:
	std::string m_Secret;
	std::string m_Token;
};

} // namespace serverbound
} // namespace packet
} // namespace mcidle

#pragma once

#include <networking/packet/Packet.hpp>

namespace mcidle {
namespace packet {
namespace clientbound {

class EncryptionRequest : public Packet 
{
public:
	EncryptionRequest() : Packet() {}
	EncryptionRequest(std::string id, std::string pubKey, std::string token) :
	Packet(), m_ServerId(id), m_PubKey(pubKey), m_Token(token) {}

	std::string PubKey() const;
	std::string Token() const;
	std::string ServerId() const;

	Packet& Serialize() override;
	void Deserialize(ByteBuffer&) override;
private:
	std::string m_ServerId;
	std::string m_PubKey;
	std::string m_Token;
};

} // namespace clientbound
} // namespace packet
} // namespace mcidle

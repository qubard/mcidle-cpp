#pragma once

#include <networking/packet/Packet.hpp>

namespace mcidle {
namespace packet {
namespace clientbound {

class EncryptionRequest : public Packet 
{
public:
	EncryptionRequest() : Packet() {}
	EncryptionRequest(std::string id, std::vector<u8>& pubKey, std::vector<u8>& token) :
	Packet(), m_ServerId(id), m_PubKey(std::move(pubKey)), m_Token(std::move(token)) {}
	Packet& Serialize() override;
	void Deserialize(ByteBuffer&) override;
private:
	std::string m_ServerId;
	std::vector<u8> m_PubKey;
	std::vector<u8> m_Token;
};

} // namespace clientbound
} // namespace packet
} // namespace mcidle

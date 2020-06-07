#pragma once

#include <networking/packet/Packet.hpp>
#include <networking/types/VarInt.hpp>

namespace mcidle {
namespace packet {
namespace serverbound {

class Handshake : public Packet 
{
public:
	Handshake(): Packet() {}
	Handshake(s32 protocol, std::string address, u16 port, s32 nextState) : 
	Packet(), m_ProtocolVersion(VarInt(protocol)), m_ServerAddress(address), m_ServerPort(port), 
	m_NextState(VarInt(nextState)) {}
	Packet& Serialize() override;
	void Deserialize(ByteBuffer&) override;
private:
	VarInt m_ProtocolVersion;
	std::string m_ServerAddress;
	u16 m_ServerPort;
	VarInt m_NextState;
};

} // namespace serverbound
} // namespace packet
} // namespace mcidle

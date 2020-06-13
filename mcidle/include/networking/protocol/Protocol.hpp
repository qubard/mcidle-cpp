#pragma once

#include <common/Typedef.hpp>

#include <networking/packet/Packet.hpp>
#include <networking/packet/serverbound/EncryptionResponse.hpp>
#include <networking/packet/serverbound/Handshake.hpp>
#include <networking/packet/serverbound/LoginStart.hpp>
#include <networking/packet/serverbound/KeepAlive.hpp>
#include <networking/packet/clientbound/ChunkData.hpp>
#include <networking/packet/clientbound/KeepAlive.hpp>
#include <networking/packet/clientbound/EncryptionRequest.hpp>
#include <networking/packet/clientbound/LoginSuccess.hpp>
#include <networking/packet/serverbound/LoginStart.hpp>
#include <networking/packet/clientbound/SetCompression.hpp>

// Packet ids change for different game versions
// When we fwd packets we dont care we just fwd the buffer

// When sending a packet we figure out the packet id
// using a functional override on a type 
// e.g virtual s32 PacketId(PackeType&)

// For receiving packets we need a mapping from integer to packet class
// So we do a functional lambda

// This way as long as we implement these mappings for game versions
// we can easily swap them in/out to support different protocol numbers
// and choose the correct protocol accordingly

namespace mcidle {

using PacketFactory = std::unique_ptr<Packet> (*)();

// Inbound packet map
using PacketMap = std::unordered_map<s32, PacketFactory>;

using ProtocolMap = std::unordered_map<s32, PacketMap>;

namespace state
{
	const s32 HANDSHAKE = 0x03;
	const s32 PLAY = 0x00;
	const s32 STATUS = 0x01;
	const s32 LOGIN = 0x02;
}

class Protocol
{
public:
	Protocol(s32 versionNumber) : m_VersionNumber(versionNumber), m_State(state::LOGIN) {}
	Protocol(ProtocolMap inboundMap, s32 versionNumber, s32 state) : m_VersionNumber(versionNumber), m_State(state)
	{
		m_InboundMap = inboundMap;
	}

	PacketMap& InboundMap()
	{
		return m_InboundMap[m_State];
	}

	s32 VersionNumber()
	{
		return m_VersionNumber;
	}

	void SetState(s32 state)
	{
		m_State = state;
	}

	virtual s32 PacketId(packet::serverbound::EncryptionResponse&) { return 0x01; }
	virtual s32 PacketId(packet::serverbound::Handshake&) { return 0x00; }
	virtual s32 PacketId(packet::serverbound::LoginStart&) { return 0x00; }
	virtual s32 PacketId(packet::clientbound::EncryptionRequest&) { return 0x01 };

	virtual s32 PacketId(packet::serverbound::KeepAlive&) { return 0x00; }

protected:
	ProtocolMap m_InboundMap;
	// Protocol number
	s32 m_VersionNumber;
	s32 m_State;
};



} // namespace mcidle

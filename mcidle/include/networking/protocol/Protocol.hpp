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
	const s32 PLAY = 0x02;
	const s32 STATUS = 0x01;
	const s32 LOGIN = 0x00;
}

// 1.12.2 map from id to packet
static ProtocolMap inboundMap_1_12_2 = 
{
	{
		state::LOGIN,
		{
			{
				0x01, []() -> std::unique_ptr<Packet> { return std::make_unique<packet::clientbound::EncryptionRequest>(); },
			},
			{
				0x02, []() -> std::unique_ptr<Packet> { return std::make_unique<packet::clientbound::LoginSuccess>(); },
			},
			{
				0x03, []() -> std::unique_ptr<Packet> { return std::make_unique<packet::clientbound::SetCompression>(); },
			}
		}
	},
	{ 
		state::PLAY,
		{
			{
				0x20, []() -> std::unique_ptr<Packet> { return std::make_unique<packet::clientbound::ChunkData>(); },
			},
			{
				0x1F, []() -> std::unique_ptr<Packet> { return std::make_unique<packet::clientbound::KeepAlive>(); },
			}
		}
	}
};

class Protocol
{
public:
	Protocol(s32 versionNumber) : m_VersionNumber(versionNumber), m_State(state::LOGIN) {}
	Protocol(ProtocolMap inboundMap, s32 versionNumber) : m_VersionNumber(versionNumber), m_State(state::LOGIN)
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

	virtual s32 PacketId(packet::serverbound::KeepAlive&) { return 0x00; }

protected:
	ProtocolMap m_InboundMap;
	s32 m_VersionNumber;
	s32 m_State;
};

class Protocol_1_12_2 : public Protocol
{
public:
	Protocol_1_12_2(s32 versionNumber) : Protocol(inboundMap_1_12_2, versionNumber)
	{
	}

	s32 PacketId(packet::serverbound::KeepAlive&) override { return 0x0B; }
};


} // namespace mcidle

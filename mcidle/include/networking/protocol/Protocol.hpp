#pragma once

#include <common/Typedef.hpp>

#include <networking/packet/Packet.hpp>
#include <networking/packet/serverbound/EncryptionResponse.hpp>
#include <networking/packet/serverbound/Handshake.hpp>
#include <networking/packet/serverbound/LoginStart.hpp>
#include <networking/packet/clientbound/ChunkData.hpp>

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

static PacketMap inboundMap_1_12_2 = 
{
	{
		0x20, []() -> std::unique_ptr<Packet> { return std::make_unique<mcidle::packet::clientbound::ChunkData>(); },
	}
};

class Protocol
{
public:
	Protocol(s32 versionNumber) : m_VersionNumber(versionNumber) {}
	Protocol(PacketMap inboundMap, s32 versionNumber) : m_VersionNumber(versionNumber)
	{
		m_InboundMap = inboundMap;
	}

	PacketMap& InboundMap()
	{
		return m_InboundMap;
	}

	s32 VersionNumber()
	{
		return m_VersionNumber;
	}

	virtual s32 PacketId(Packet&) { return -1; }
	virtual s32 PacketId(mcidle::packet::serverbound::EncryptionResponse&) { return 0x01; }
	virtual s32 PacketId(mcidle::packet::serverbound::Handshake&) { return 0x00; }
	virtual s32 PacketId(mcidle::packet::serverbound::LoginStart&) { return 0x00; }

protected:
	PacketMap m_InboundMap;
	s32 m_VersionNumber;
};

class Protocol_1_12_2 : public Protocol
{
public:
	Protocol_1_12_2(s32 versionNumber) : Protocol(inboundMap_1_12_2, versionNumber)
	{
	}
};


} // namespace mcidle

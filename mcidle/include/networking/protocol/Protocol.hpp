#pragma once

#include <common/Typedef.hpp>

#include <networking/packet/Packet.hpp>
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
	Protocol() {}
	Protocol(PacketMap inboundMap) 
	{
		m_InboundMap = inboundMap;
	}

	PacketMap& InboundMap()
	{
		return m_InboundMap;
	}

	//virtual s32 PacketId(mcidle::packet::serverbound::EncryptionResponse&) { return 0x01; }
	//virtual s32 PacketId(mcidle::packet::serverbound::Handshake&) { return 0x00; }
	//virtual s32 PacketId(mcidle::packet::serverbound::LoginStart&) { return 0x00; }

protected:
	PacketMap m_InboundMap;
};

class Protocol_1_12_2 : public Protocol
{
public:
	Protocol_1_12_2() : Protocol(inboundMap_1_12_2)
	{
	}
};


} // namespace mcidle

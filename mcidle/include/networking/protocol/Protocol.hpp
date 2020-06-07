#pragma once

#include <common/Typedef.hpp>
#include <networking/packet/serverbound/EncryptionResponse.hpp>
#include <networking/packet/serverbound/Handshake.hpp>
#include <networking/packet/serverbound/LoginStart.hpp>
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

class Protocol
{
public:
	Protocol() {}

	virtual s32 PacketId(mcidle::packet::serverbound::EncryptionResponse&) { return 0x01; }
	virtual s32 PacketId(mcidle::packet::serverbound::Handshake&) { return 0x00; }
	virtual s32 PacketId(mcidle::packet::serverbound::LoginStart&) { return 0x00; }
};

class Protocol_1_12_2 : public Protocol
{
};

} // namespace mcidle

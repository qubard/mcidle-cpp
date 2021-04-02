#pragma once

#include <networking/packet/Packet.hpp>
#include <networking/protocol/Protocol.hpp>

namespace mcidle
{

template <typename T>
T & SerializeWrite(T & packet, Protocol & protocol, s32 compression)
{
	auto id = protocol.PacketId(packet);
	packet.SetId(id)
		.SetProtocol(protocol.VersionNumber())
		.Serialize()
		.Write(compression);
	return packet;
}

// Need this to write and serialize packets without repetition
template <typename T, typename... Ts>
std::shared_ptr<T>
CreatePacket(Protocol & protocol, s32 compression, Ts... args)
{
	auto packet = std::make_shared<T>(args...);
	SerializeWrite(*packet, protocol, compression);
	return packet;
}

}  // namespace mcidle

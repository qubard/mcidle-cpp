#include <networking/packet/Packet.hpp>
#include <networking/types/VarInt.hpp>

#include <zlib.h>

namespace mcidle {

Packet& Packet::Serialize()
{
	return *this;
}

void Packet::Deserialize(ByteBuffer& buf)
{
}

Packet& Packet::SetCompression(s32 threshold)
{
	m_Compression = threshold;
	return *this;
}

Packet& Packet::SetProtocol(s32 protocol)
{
	m_Protocol = protocol;
	return *this;
}

Packet& Packet::SetId(s32 id)
{
	m_Id = id;
	return *this;
}

inline void Packet::Compress(ByteBuffer& buf)
{
}

std::shared_ptr<ByteBuffer> Packet::Buffer()
{
	return m_PacketBuf;
}

std::string Packet::Hex()
{
	return m_PacketBuf->Hex();
}

Packet& Packet::Reserve(std::size_t capacity)
{
	m_FieldBuf->Reserve(capacity);
	return *this;
}

// Assume that Serialize() has already been called
// and `m_Buf` is not empty

// If we have the size of `m_FieldBuf` we can speed this up
// by just knowing how much space we'll need exactly for
// packet id + data length + packet length then we only do one copy
void Packet::Write()
{
	if (m_FieldBuf->Size() == 0)
	{
		throw std::runtime_error("Cannot write empty packet, serialize fields first!");
	}

	// Reserve enough space for `m_FieldBuf` + a single VarInt (5-10% speedup)
	//mcidle::ByteBuffer pktBuf((std::size_t)(5 + m_FieldBuf->Size()));
	// Write the packet's id
	auto id = VarInt(m_Id);

	std::size_t uncompressedLen = m_FieldBuf->Size() + id.Size();

	// Compress the buffer if needed
	/*if (m_CompressionThreshold > 0 && pktBuf.Size() >= m_CompressionThreshold)
	{
		Compress(pktBuf);
	}*/

	// `pktBuf` with length prepended
	// reserve as many bytes we would need to write the entirety of `pktBuf` + VarInt
	mcidle::ByteBuffer lenPktBuf(uncompressedLen + 5);
	lenPktBuf << VarInt(uncompressedLen);
	lenPktBuf << id;
	lenPktBuf << *m_FieldBuf; // Unfortunately this has to do a copy

	// Re-assign the final outbound packet buffer without
	// compression to the packet buffer prepended with `m_Id`
	// Use move semantics here so we avoid a copy
	m_PacketBuf = std::make_shared<ByteBuffer>(std::move(lenPktBuf));
}

}

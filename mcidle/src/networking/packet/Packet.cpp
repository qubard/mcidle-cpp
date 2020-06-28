#include <common/Compression.hpp>
#include <networking/packet/Packet.hpp>
#include <networking/protocol/Protocol.hpp>
#include <networking/types/VarInt.hpp>

namespace mcidle {

Packet::Packet() : m_Id(-1), m_Protocol(-1), m_FieldBuf(std::make_unique<ByteBuffer>()), m_RawRecBuf(nullptr)
{
}

Packet::Packet(std::shared_ptr<ByteBuffer> fieldBuf)
    : m_Id(-1), m_Protocol(-1), m_FieldBuf(fieldBuf), m_RawRecBuf(nullptr)
{
}

Packet& Packet::Serialize()
{
	return *this;
}

void Packet::Deserialize(ByteBuffer& buf)
{
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

void Packet::Mutate(GameState &state)
{
}

std::shared_ptr<Packet> Packet::Response(Protocol &protocol, s32 compression)
{
    return nullptr;
}

std::shared_ptr<ByteBuffer> Packet::Buffer()
{
	return m_PacketBuf;
}

Packet& Packet::Reserve(std::size_t capacity)
{
	m_FieldBuf->Reserve(capacity);
	return *this;
}

Packet& Packet::SetRawBuffer(std::shared_ptr<ByteBuffer> buf)
{
	m_RawRecBuf = buf;
	return *this;
}

Packet& Packet::SetFieldBuffer(std::shared_ptr<ByteBuffer> buf)
{
	m_FieldBuf = buf;
	return *this;
}

std::shared_ptr<ByteBuffer> Packet::RawBuffer()
{
	return m_RawRecBuf;
}

std::shared_ptr<ByteBuffer> Packet::FieldBuffer()
{
	return m_FieldBuf;
}

s32 Packet::Id()
{
	return m_Id;
}

void Packet::Write(s32 compressionThreshold)
{
	if (m_FieldBuf->Size() == 0)
	{
		throw std::runtime_error("Cannot write empty packet, serialize fields first!");
	}

	ByteBuffer outBuf;

	// The packet's id
	auto id = VarInt(m_Id);

	// Packet ID + Data (FieldBuf)
	ByteBuffer packetBuf;

	// Estimate what the size of the ByteBuffer should be worst case
	packetBuf.Resize(m_FieldBuf->Size() + 5);
	packetBuf << id;
	packetBuf << *m_FieldBuf;

	std::size_t uncompressedLen = packetBuf.WriteSize();

	// Packet is compressed
	if (compressionThreshold > 0 && uncompressedLen >= compressionThreshold)
	{
		VarInt dataLength(uncompressedLen);
		auto compressedData = Compress(packetBuf);
		VarInt packetLength(compressedData->WriteSize() + dataLength.Size());
		outBuf.Resize((u64)packetLength.Size() + packetLength.Value());
		outBuf << packetLength;
		outBuf << dataLength;
		outBuf << compressedData;
	}
	else {
		// Compression is disabled or data length is 0
		VarInt packetLength(uncompressedLen + (compressionThreshold >= 0 ? 1 : 0));
		outBuf.Resize(packetLength.Value());
		outBuf << packetLength;
		if (compressionThreshold >= 0)
			outBuf << VarInt(0);
		outBuf << packetBuf;
	}

	m_PacketBuf = std::make_shared<ByteBuffer>(std::move(outBuf));
}

}  // namespace mcidle

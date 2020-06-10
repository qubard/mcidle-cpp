#include <networking/Connection.hpp>
#include <networking/types/VarInt.hpp>
#include <zlib.h>

namespace mcidle {

inline bool Connection::PrepareRead()
{
	auto asioBuf = boost::asio::buffer(m_ReadBuf.Front(), m_ReadBuf.Size());
	std::size_t recLen = m_Socket->Recv(asioBuf);

	// Encryption enabled
	if (recLen > 0 && m_Aes != nullptr)
	{
		// Decrypt the buffer and replace it
		m_ReadBuf = std::move(*m_Aes->Decrypt(m_ReadBuf, recLen));
	}

	// Store the last recorded size so we can calculate
	// if we've received enough bytes
	m_LastRecSize = recLen;

	return recLen > 0;
}

Connection& Connection::SetCompression(s32 compression)
{
	m_Compression = compression;
	return *this;
}

inline std::shared_ptr<ByteBuffer> Connection::Decompress(std::shared_ptr<ByteBuffer>& buf)
{
	mcidle::VarInt compressLen;
	*buf >> compressLen;
	// Uncompressed length
	auto len = compressLen.Value();

	// Is the buffer compressed?
	if (compressLen.Value() > 0)
	{
		auto uncompressed = std::make_shared<mcidle::ByteBuffer>();
		uncompressed->Resize(compressLen.Value());
		uncompress((Bytef*)uncompressed->Front(), (uLongf*)&len,
			(const Bytef*)&buf->Peek(), (uLong)(buf->Size() - buf->ReadOffset()));
		return std::move(uncompressed);
	}
	return nullptr;
}

std::shared_ptr<ByteBuffer> Connection::ReadBuffer()
{
	// Read a new chunk if we don't have any more packets
	if (m_ReadBuf.ReadOffset() >= m_LastRecSize || m_LastRecSize == 0)
	{
		// Not enough bytes
		if (!PrepareRead())
			return nullptr;
	}

	VarInt packetLen;
	try
	{
		m_ReadBuf >> packetLen;
	}
	// If we accidentally can't read the length, return a nullptr
	// Hopefully this never happens, but it could in theory
	catch (std::runtime_error e)
	{
		return nullptr;
	}

	if (packetLen.Value() <= 0)
	{
		throw std::runtime_error("Received invalid packet length <= 0");
	}

	auto lenSize = packetLen.Size();
	// The remaining bytes in the read buffer after reading length
	s32 remaining = m_LastRecSize - m_ReadBuf.ReadOffset();

	// Allocate the individual packet output buffer
	auto packetBuf = std::make_shared<ByteBuffer>();
	packetBuf->Resize(packetLen.Value() + lenSize);
	m_ReadBuf.SeekRead(m_ReadBuf.ReadOffset() - lenSize);

	// Packet doesn't fit in the buffer, do an additional read call
	if (remaining < packetLen.Value())
	{
		s32 extra = packetLen.Value() - remaining;

		m_ReadBuf.Read(*packetBuf, remaining + lenSize);

		// Read bytes into the back until we have enough
		ByteBuffer extraBuf;
		extraBuf.Resize(extra);

		auto asioBuf = boost::asio::buffer(extraBuf.Front(), extraBuf.Size());

		if (m_Socket->Read(asioBuf) == 0)
			return nullptr;

		if (m_Aes != nullptr)
		{
			auto decrypt = m_Aes->Decrypt(extraBuf, extraBuf.Size());
			// Combine the output buffer with the rest of the decrypted bytes
			decrypt->Read(*packetBuf, decrypt->Size());
		}
		else
		{
			// Packet isn't encrypted, append the raw bytes
			extraBuf.Read(*packetBuf, extraBuf.Size());
		}
	}
	else
	{
		// Packet fits in the buffer, copy the bytes over
		m_ReadBuf.Read(*packetBuf, packetLen.Value() + lenSize);
	}

	// Seek off the length so it doesn't have to be read later
	// but still exists in the buffer
	packetBuf->SeekRead(lenSize);

	return packetBuf;
}

std::unique_ptr<Packet> Connection::ReadPacket()
{
	auto packetBuf = ReadBuffer();

	if (packetBuf == nullptr)
		return nullptr;

	auto packet = std::make_unique<Packet>();
	packet->SetFieldBuffer(packetBuf);

	// Try to decompress the packet
	if (m_Compression > 0)
	{
		auto decompressed = Decompress(packetBuf);
		if (decompressed != nullptr)
		{
			packet->SetFieldBuffer(decompressed);
		}
	}

	// Field buf is valid, read the packet ID and set it
	mcidle::VarInt id;
	*packet->FieldBuffer() >> id;
	packet->SetId(id.Value());

	// The raw buffer is the uncompressed field buffer
	// useful for forwarding packets without having
	// to re-compress them
	packet->SetRawBuffer(packetBuf);

	auto inboundMap = m_Protocol->InboundMap();

	// Lookup the inbound map packet given the protocol
	// and deserialize the packet into it
	if (inboundMap.find(id.Value()) != inboundMap.end())
	{
		auto mappedPacket = inboundMap[id.Value()]();
		mappedPacket->SetFieldBuffer(packet->FieldBuffer());
		mappedPacket->SetRawBuffer(packet->RawBuffer());
		mappedPacket->SetId(packet->Id());
		mappedPacket->Deserialize(*packet->FieldBuffer());
		return mappedPacket;
	}

	return packet;
}

Connection& Connection::SetAes(std::unique_ptr<AesCtx>& ctx)
{
	m_Aes = std::move(ctx);
	return *this;
}

}


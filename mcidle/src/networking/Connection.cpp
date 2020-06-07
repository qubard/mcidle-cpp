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

inline std::unique_ptr<ByteBuffer> Connection::Decompress(std::unique_ptr<ByteBuffer>& buf)
{
	mcidle::VarInt compressLen;
	*buf >> compressLen;
	// Uncompressed length
	auto len = compressLen.Value();

	// Is the buffer compressed?
	if (compressLen.Value() > 0)
	{
		auto uncompressed = std::make_unique<mcidle::ByteBuffer>();
		uncompressed->Resize(compressLen.Value());
		uncompress((Bytef*)uncompressed->Front(), (uLongf*)&len,
			(const Bytef*)&buf->Peek(), (uLong)(buf->Size() - buf->ReadOffset()));
		return std::move(uncompressed);
	}
	return nullptr;
}

std::unique_ptr<ByteBuffer> Connection::ReadPacket()
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

	// The remaining bytes in the read buffer after reading length
	s32 remaining = m_LastRecSize - m_ReadBuf.ReadOffset();

	// Allocate the individual packet output buffer
	auto packetOut = std::make_unique<ByteBuffer>();
	packetOut->Resize(packetLen.Value());

	// Packet doesn't fit in the buffer, do an additional read call
	if (remaining < packetLen.Value())
	{
		s32 extra = packetLen.Value() - remaining;

		m_ReadBuf.Read(packetOut->Front(), remaining);

		// Read bytes into the back until we have enough
		ByteBuffer extraBuf;
		extraBuf.Resize(extra);

		auto asioBuf = boost::asio::buffer(extraBuf.Front(), extraBuf.Size());

		// Block until we have the rest of the packet
		if (m_Socket->Read(asioBuf) == 0)
			return nullptr;

		if (m_Aes != nullptr)
		{
			auto decrypt = m_Aes->Decrypt(extraBuf, extraBuf.Size());
			// Combine the output buffer with the rest of the decrypted bytes
			decrypt->Read(packetOut->Front() + remaining, decrypt->Size());
		}
		else 
		{
			// Packet isn't encrypted, append the raw bytes
			extraBuf.Read(packetOut->Front() + remaining, extraBuf.Size());
		}
	}
	else
	{
		// Packet fits in the buffer, copy the bytes over
		m_ReadBuf.Read(packetOut->Front(), packetLen.Value());
	}

	// Try to decompress the packet
	if (m_Compression > 0)
	{
		auto decompressed = Decompress(packetOut);
		if (decompressed != nullptr)
			packetOut = std::move(decompressed);
	}

	return packetOut;
}

Connection& Connection::SetAes(std::unique_ptr<AesCtx>& ctx)
{
	m_Aes = std::move(ctx);
	return *this;
}

}


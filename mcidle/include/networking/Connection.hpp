#pragma once

#include <networking/TCPSocket.hpp>
#include <networking/encryption/AesCtx.hpp>
#include <networking/packet/Packet.hpp>
#include <networking/protocol/Protocol.hpp>

namespace mcidle {

class Connection
{
public:
	Connection() = default;
	Connection(std::shared_ptr<TCPSocket> socket, 
		std::unique_ptr<Protocol> protocol, 
		std::size_t size):
		m_Socket(std::move(socket)), m_ReadSize(size), m_Aes(nullptr), m_ReadBuf(size),
		m_LastRecSize(0), m_Protocol(std::move(protocol)), m_Compression(-1) {
		m_ReadBuf.Resize(size);
	}

	// Equivalent to enabling encryption
	Connection& SetAes(std::unique_ptr<AesCtx>&);
	Connection& SetCompression(s32);

	template <typename T>
	void SendPacket(T&& packet)
	{
		auto id = m_Protocol->PacketId(packet);
		packet.
			SetId(id).
			SetProtocol(m_Protocol->VersionNumber()).
			Serialize().Write(m_Compression);

		auto buf = packet.Buffer();

		boost::asio::mutable_buffer mutBuf;
		if (m_Aes != nullptr)
			buf = std::move(m_Aes->Encrypt(*buf, buf->Size()));

		mutBuf = boost::asio::buffer(buf->Front(), buf->Size());
		m_Socket->Send(mutBuf);
	}

	// Read a packet from the socket as a buffer
	// This is a raw packet, still needs to be decompressed
	std::unique_ptr<Packet> ReadPacket();
	std::shared_ptr<ByteBuffer> ReadBuffer();
private:
	// Prepare `m_ReadBuf` for an actual read (read 4k bytes)
	inline bool PrepareRead();
	// Decompress a byte buffer and return the decompressed buffer
	inline std::shared_ptr<ByteBuffer> Decompress(std::shared_ptr<ByteBuffer>&);

	std::unique_ptr<Protocol> m_Protocol;
	std::shared_ptr<TCPSocket> m_Socket;
	std::unique_ptr<AesCtx> m_Aes;

	s32 m_Compression;

	// The read buffer for incoming packet data
	ByteBuffer m_ReadBuf;
	std::size_t m_ReadSize;
	std::size_t m_LastRecSize;
};

} // namespace mcidle

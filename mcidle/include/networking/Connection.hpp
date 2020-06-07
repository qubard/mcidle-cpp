#pragma once

#include <networking/TCPSocket.hpp>
#include <networking/encryption/AesCtx.hpp>
#include <networking/protocol/Protocol.hpp>

#include <iostream>

namespace mcidle {

class Connection
{
public:
	Connection() = default;
	Connection(std::unique_ptr<TCPSocket> socket, 
		std::unique_ptr<Protocol> protocol, 
		std::size_t size):
		m_Socket(std::move(socket)), m_ReadSize(size), m_Aes(nullptr), m_ReadBuf(size),
		m_LastRecSize(0), m_Protocol(std::move(protocol)), m_Compression(-1) {
		m_ReadBuf.Resize(size);
	}

	// Equivalent to enabling encryption
	Connection& SetAes(std::unique_ptr<AesCtx>&);
	Connection& SetCompression(s32);

	// Read a packet from the socket as a buffer
	// This is a raw packet, still needs to be decompressed
	std::unique_ptr<ByteBuffer> ReadPacket();
private:
	// Prepare `m_ReadBuf` for an actual read (read 4k bytes)
	inline bool PrepareRead();
	// Decompress a packet byte buffer and return the decompressed buffer
	inline std::unique_ptr<ByteBuffer> Decompress(std::unique_ptr<ByteBuffer>&);

	std::unique_ptr<Protocol> m_Protocol;
	std::unique_ptr<TCPSocket> m_Socket;
	std::unique_ptr<AesCtx> m_Aes;

	s32 m_Compression;

	// The read buffer for incoming packet data
	ByteBuffer m_ReadBuf;
	std::size_t m_ReadSize;
	std::size_t m_LastRecSize;
};

} // namespace mcidle

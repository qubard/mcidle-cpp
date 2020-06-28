#pragma once

#include <memory>

#include <networking/TCPSocket.hpp>
#include <networking/encryption/AesCtx.hpp>
#include <networking/packet/Packet.hpp>

#include <networking/packet/Serialize.hpp>

namespace mcidle {

class Connection
{
public:
    Connection(std::unique_ptr<TCPSocket>, std::shared_ptr<mcidle::Protocol>, std::size_t);

    // Equivalent to enabling encryption
    Connection &SetAes(std::unique_ptr<AesCtx> &);
    Connection &SetCompression(s32);
    s32 Compression();
    Protocol &Protocol();

    // Send a packet without FullWrite
    void SendPacketSimple(Packet &packet);

    template <typename T>
    void SendPacket(T &&packet)
    {
        SerializeWrite(packet, *m_Protocol, m_Compression);
        SendPacketSimple(packet);
    }

    // Read a packet from the socket as a buffer
    // This is a raw packet, still needs to be decompressed
    std::unique_ptr<Packet> ReadPacket();

private:
	std::shared_ptr<ByteBuffer> ReadBuffer();
	// Prepare `m_ReadBuf` for an actual read (read 4k bytes)
	inline bool PrepareRead();

        std::shared_ptr<mcidle::Protocol> m_Protocol;
        std::unique_ptr<TCPSocket> m_Socket;
        std::unique_ptr<AesCtx> m_Aes;

        s32 m_Compression;

        // The read buffer for incoming packet data
        ByteBuffer m_ReadBuf;
        // The size of each chunked read call
	std::size_t m_ReadSize;
	std::size_t m_LastRecSize;
};

}  // namespace mcidle

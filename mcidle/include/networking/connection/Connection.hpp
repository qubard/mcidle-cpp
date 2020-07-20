#pragma once

#include <memory>

#include <networking/TCPSocket.hpp>
#include <networking/encryption/AesCtx.hpp>
#include <networking/packet/Packet.hpp>

#include <networking/packet/Serialize.hpp>
#include <util/Yggdrasil.hpp>

namespace mcidle {

class Connection
{
public:
    Connection(std::unique_ptr<TCPSocket>, std::shared_ptr<mcidle::Protocol>, std::shared_ptr<mcidle::game::GameState>, std::size_t);

    // Equivalent to enabling encryption
    Connection &SetAes(std::unique_ptr<AesCtx> &);
    Connection &SetCompression(s32);
    s32 Compression();
    mcidle::Protocol &Protocol();

    // Hook to be called before the connection is ran/used
    virtual bool Setup(mcidle::util::Yggdrasil &);

    // Send an already written packet without FullWrite
    std::size_t SendPacketFwd(Packet &packet);
    // Send a byte buffer
    std::size_t SendBuffer(std::shared_ptr<ByteBuffer>&);

    template <typename T>
    std::size_t SendPacket(T &&packet)
    {
        SerializeWrite(packet, *m_Protocol, m_Compression);
        return SendPacketFwd(packet);
    }

    // Read a packet from the socket as a buffer
    // This is a raw packet, still needs to be decompressed
    std::shared_ptr<Packet> ReadPacket();
    std::shared_ptr<mcidle::Protocol> m_Protocol;

private:
	std::shared_ptr<ByteBuffer> ReadBuffer();
	// Prepare `m_ReadBuf` for an actual read (read 4k bytes)
	inline bool PrepareRead();

    s32 m_Compression;

    // The read buffer for incoming packet data
    ByteBuffer m_ReadBuf;
    // The size of each chunked read call
    std::size_t m_ReadSize;
	std::size_t m_LastRecSize;

    std::unique_ptr<TCPSocket> m_Socket;
    std::unique_ptr<AesCtx> m_Aes;
    std::shared_ptr<mcidle::game::GameState> m_State;
};

}  // namespace mcidle

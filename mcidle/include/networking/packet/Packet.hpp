#pragma once 

#include <memory>
#include <networking/ByteBuffer.hpp>

namespace mcidle {

class Packet
{
public:
	Packet() : m_Id(-1), m_Compression(-1),
		m_Protocol(-1), m_FieldBuf(std::make_unique<ByteBuffer>()) 
	{
	}
	
	Packet(std::unique_ptr<ByteBuffer>& fieldBuf) : m_Id(-1), m_Compression(-1),
		m_Protocol(-1), m_FieldBuf(std::move(fieldBuf))
	{
	}

	// It's better to take the compression and protocol later
	// because muddying the constructor causes a lot of confusion
	// when we add fields to the constructor later
	Packet& SetCompression(s32);
	Packet& SetProtocol(s32);
	Packet& SetId(s32);

	// Serialize the packet's fields into `m_Buf`
	virtual Packet& Serialize();
	// Deserialize the packet from a ByteBuffer& into fields
	virtual void Deserialize(ByteBuffer&);

	// Reserve space in `m_FieldBuf`
	Packet& Reserve(std::size_t);

	// Instead of exposing a boost::asio::mutable_buffer
	// we can expose a pointer to the complete packet
	std::shared_ptr<ByteBuffer> Buffer();

	std::string Hex();

	// Write the packet and compress if necessary
	void Write();
protected:
	s32 m_Protocol;
	// The packet's VarInt encoded id
	s32 m_Id;
	// The compression threshold (-1 if disabled)
	s32 m_Compression;
	// Compress the buffer with zlib given the threshold
	inline void Compress(ByteBuffer& buf);
	// The field or data buffer for serialization
	std::unique_ptr<ByteBuffer> m_FieldBuf;

private:
	// The final packet buffer that gets sent outbound
	std::shared_ptr<ByteBuffer> m_PacketBuf;
};

}
#pragma once 

#include <memory>
#include <networking/ByteBuffer.hpp>

namespace mcidle {

class Packet
{
public:
	Packet() : m_Id(-1), m_Compression(-1),
		m_Protocol(-1), m_FieldBuf(std::make_unique<ByteBuffer>()),
		m_RawRecBuf(nullptr)
	{
	}
	
	Packet(std::shared_ptr<ByteBuffer> fieldBuf) : m_Id(-1), m_Compression(-1),
		m_Protocol(-1), m_FieldBuf(fieldBuf), m_RawRecBuf(nullptr)
	{
	}

	// It's better to take the compression and protocol later
	// because muddying the constructor causes a lot of confusion
	// when we add fields to the constructor later
	Packet& SetCompression(s32);
	Packet& SetProtocol(s32);
	Packet& SetId(s32);
	Packet& SetRawBuffer(std::shared_ptr<ByteBuffer>);
	Packet& SetFieldBuffer(std::shared_ptr<ByteBuffer>);

	std::shared_ptr<ByteBuffer> RawBuffer();

	// Serialize the packet's fields into `m_Buf`
	virtual Packet& Serialize();
	// Deserialize the packet from a ByteBuffer& into fields
	virtual void Deserialize(ByteBuffer&);

	// Reserve space in `m_FieldBuf`
	Packet& Reserve(std::size_t);

	// Exposes a pointer to the serialized buffer
	std::shared_ptr<ByteBuffer>	Buffer();
	std::shared_ptr<ByteBuffer> FieldBuffer();

	s32 Id();

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
	std::shared_ptr<ByteBuffer> m_FieldBuf;
	
private:
	// The final serialized outbound packet buffer
	std::shared_ptr<ByteBuffer> m_PacketBuf;
	// The raw inbound packet buffer
	std::shared_ptr<ByteBuffer> m_RawRecBuf;
};

}
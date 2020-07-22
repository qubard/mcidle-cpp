#include <networking/ByteBuffer.hpp>
#include <networking/types/VarInt.hpp>

#include <iostream>

namespace mcidle {

ByteBuffer::ByteBuffer() : m_BigEndian(true), m_ReadOffset(0), m_WriteOffset(0)
{
}

ByteBuffer::ByteBuffer(std::vector<u8>& data) : m_BigEndian(true), m_ReadOffset(0),
m_WriteOffset(0)
{
	m_Data = std::move(data);
}

ByteBuffer::ByteBuffer(ByteBuffer&& buf) : m_BigEndian(buf.m_BigEndian), 
m_ReadOffset(buf.m_ReadOffset), m_Data(std::move(buf.m_Data)), m_WriteOffset(buf.m_WriteOffset)
{
}

ByteBuffer::ByteBuffer(std::size_t capacity, bool bigEndian=true): m_BigEndian(bigEndian), 
m_ReadOffset(0), m_WriteOffset(0)
{
	// Reserve `capacity` bytes
	m_Data.reserve(capacity);
}

ByteBuffer::ByteBuffer(std::size_t capacity) : m_BigEndian(true), m_ReadOffset(0), m_WriteOffset(0)
{
	m_Data.reserve(capacity);
}

bool ByteBuffer::Avail() const
{
	return m_ReadOffset < m_Data.size();
}

void ByteBuffer::SeekRead(std::size_t offset)
{
	m_ReadOffset = offset;
}

void ByteBuffer::SeekWrite(std::size_t offset)
{
	m_WriteOffset = offset;
}

// Copy bytes from `buf` into the current buffer
ByteBuffer& ByteBuffer::operator<<(ByteBuffer& buf)
{
	Write(buf.Front(), buf.Size());
	return *this;
}

std::size_t ByteBuffer::ReadOffset() const
{
	return m_ReadOffset;
}

ByteBuffer& ByteBuffer::operator<<(std::string& str)
{
	// Write the string length as a VarInt
	Write(VarInt(str.size()));
	if (str.size() > 0)
	{
		// Write the actual bytes in the string
		Write((u8*)&str[0], str.size());
	}
	return *this;
}

ByteBuffer& ByteBuffer::operator<<(std::vector<u8>& vec)
{
	Write(VarInt(vec.size()));
	if (vec.size() > 0)
		Write((const u8*)vec.data(), vec.size());
	return *this;
}

ByteBuffer& ByteBuffer::operator<<(std::vector<u64>& vec)
{
	Write(VarInt(vec.size()));
	if (vec.size() > 0)
		Write((const u8*)vec.data(), sizeof(u64) * vec.size());
	return *this;
}

ByteBuffer& ByteBuffer::operator>>(std::vector<u64>& vec)
{
	VarInt len = Read<VarInt>();
	auto size = len.Value() * sizeof(u64);
	vec.resize(vec.size() + size);
	Read((u8*)&vec[vec.size() - size], size);
	return *this;
}

ByteBuffer& ByteBuffer::operator>>(std::vector<u8>& vec)
{
	VarInt len = Read<VarInt>();
	auto size = len.Value() * sizeof(u8);
	vec.resize(vec.size() + size);
	Read((u8*)&vec[vec.size() - size], size);
	return *this;
}

ByteBuffer& ByteBuffer::operator<<(const char* buf)
{
	// Convert `buf` to a string first
	std::string str(buf, strlen(buf));
	*this << str;
	return *this;
}

ByteBuffer& ByteBuffer::operator<<(std::string&& str)
{
	// Can't take the address of an rvalue so we 
	// convert to an lvalue first with constness
	*this << str;
	return *this;
}

void ByteBuffer::Reserve(std::size_t capacity)
{
	m_Data.reserve(capacity);
}

u8& ByteBuffer::Peek()
{
	if (m_ReadOffset >= m_Data.capacity())
	{
		throw std::runtime_error("Read offset exceeds capacity");
	}
	return m_Data[m_ReadOffset];
}

u8* ByteBuffer::Front() 
{
	return m_Data.data();
}

u8* ByteBuffer::Back() 
{
	return m_Data.data() + m_Data.size();
}

ByteBuffer& ByteBuffer::operator>>(std::string& str)
{
	VarInt len = Read<VarInt>();
	std::string tmp;
	tmp.resize(len.Value());
	Read((u8*)tmp.c_str(), len.Value());
	str = std::move(tmp);
	return *this;
}

std::size_t ByteBuffer::Size() const
{
	return m_Data.size();
}

std::size_t ByteBuffer::WriteSize() const
{
	return m_WriteOffset;
}

void ByteBuffer::Clear()
{
	m_Data.clear();
	m_WriteOffset = 0;
	m_ReadOffset = 0;
}

ByteBuffer &ByteBuffer::Resize(u64 size)
{
	m_Data.resize(size);
	return *this;
}

const std::string ByteBuffer::Hex()
{
	if (m_Data.size() == 0) return "";
	std::string hex;
	boost::algorithm::hex(m_Data, std::back_inserter(hex));

	std::string res;
	for (size_t i = 0; i < hex.length(); i += 2) {
		res += hex.substr(i, 2);
		if (i + 2 < hex.length()) res += ' ';
	}
	return res;
}

u8& ByteBuffer::operator[](std::size_t idx)
{
	return m_Data[idx];
}

ByteBuffer& ByteBuffer::operator=(ByteBuffer&& other)
{
	if (this != &other)
	{
		m_Data = std::move(other.m_Data);
		m_ReadOffset = other.m_ReadOffset;
		m_BigEndian = other.m_BigEndian;
	}
	return *this;
}

ByteBuffer& ByteBuffer::operator=(ByteBuffer& other)
{
	m_Data = other.m_Data;
	m_ReadOffset = other.m_ReadOffset;
	m_BigEndian = other.m_BigEndian;
	return *this;
}

// Write `len` bytes from `src`
void ByteBuffer::Write(const u8* src, std::size_t size)
{
	if (m_WriteOffset + size > m_Data.size())
		m_Data.resize(m_WriteOffset + size);
	memcpy(&m_Data[m_WriteOffset], src, size);
	m_WriteOffset += size;
}

void ByteBuffer::Read(u8* dst, std::size_t size)
{
	if (m_ReadOffset + size > m_Data.size())
	{
		throw std::runtime_error("Invalid read! Not enough bytes");
	}
	memcpy(dst, m_Data.data() + m_ReadOffset, size);
	m_ReadOffset += size;
}

void ByteBuffer::Read(ByteBuffer& buf, std::size_t size)
{
	if (buf.m_WriteOffset + size > buf.Size())
		buf.Resize(buf.m_WriteOffset + size);
	Read(&buf.m_Data[buf.m_WriteOffset], size);
	buf.m_WriteOffset += size;
}

}  // namespace mcidle

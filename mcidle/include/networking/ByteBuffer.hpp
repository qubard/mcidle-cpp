#pragma once

#include <vector>
#include <string>

#include <common/Typedef.hpp>
#include <boost/algorithm/hex.hpp>

namespace mcidle {

class ByteBuffer
{
public:
	ByteBuffer();
	ByteBuffer(bool);
	ByteBuffer(std::size_t, bool);
	ByteBuffer(std::size_t);
	ByteBuffer(ByteBuffer&) = default;
	ByteBuffer(ByteBuffer&&);
	ByteBuffer(std::vector<u8>&);

	void Reserve(std::size_t);
        ByteBuffer &Resize(u64);
        void Clear();
        void SeekRead(std::size_t);
        std::size_t Size() const;
        std::size_t WriteSize() const;
        const std::string Hex();

        void SeekWrite(std::size_t);

        // Return a reference to the byte at the current read offset
	u8& Peek();
	u8* Front();
	u8* Back();

	std::size_t ReadOffset() const;

	bool Avail() const;

	void Write(const u8*, std::size_t);

	// Read `size` bytes into a destination address
	void Read(u8*, std::size_t);
	// Read `size` bytes into a desitination buffer
	void Read(ByteBuffer&, std::size_t);

	u8& operator[](std::size_t);

	ByteBuffer& operator=(ByteBuffer&&);
	ByteBuffer& operator=(ByteBuffer&);

	ByteBuffer& operator<<(ByteBuffer&);
	ByteBuffer& operator<<(std::string&);
	ByteBuffer& operator<<(std::string&&);
	ByteBuffer& operator<<(const char*);
	ByteBuffer& operator<<(std::vector<u8>&);

	ByteBuffer& operator>>(std::vector<u64>& vec);
	ByteBuffer& operator>>(std::vector<u8>& vec);

	ByteBuffer& operator>>(std::string&);

	// For generic types always make a copy
	// We won't worry about move semantics here
	// Since we don't expect types that implement them
	// If we desire that (VarInt, VarLong) we implement
	// those overloads
	template <typename T>
	ByteBuffer& operator<<(T&& val)
	{
		Write(std::forward<T>(val));
		return *this;
	}

	template <typename T>
	ByteBuffer& operator>>(T& val)
	{
		val = Read<T>();
		return *this;
	}

	template <typename T>
	T Read() 
	{
		// Ensure there are enough bytes to be read
		if (m_ReadOffset + sizeof(T) > m_Data.size())
		{
			throw std::runtime_error("Not enough bytes during templated read!");
		}

		T val = *(T*)&m_Data[m_ReadOffset];
		// Convert to little endian
		if (m_BigEndian)
			std::reverse((u8*)&val, (u8*)&val + sizeof(T));
		m_ReadOffset += sizeof(T);
		return val;
	}

	template <typename T>
	void Write(const T val)
	{
		// Reverse to big-endian
		// all modern processors are little endian (intel, amd)
		// so reverse byte order
		if (m_BigEndian)
			std::reverse((u8*)&val, (u8*)&val + sizeof(T));
		Append(val);
	}

private:
	template <typename T>
	void Append(const T val)
	{
		std::size_t size = sizeof(val);
		if (m_WriteOffset + size > m_Data.size())
			m_Data.resize(m_WriteOffset + size);
		// Copy multiple bytes to the buffer
		memcpy(&m_Data[m_WriteOffset], &val, size);
		m_WriteOffset += size;
	}

	std::vector<u8> m_Data;
	std::size_t m_WriteOffset;
	std::size_t m_ReadOffset;
	// Endianness of byte insertion order
	bool m_BigEndian;
};

class VarInt;
template<>
VarInt ByteBuffer::Read();

}

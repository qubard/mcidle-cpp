#include <networking/types/VarInt.hpp>

namespace mcidle {

VarInt::VarInt() : Var<s32>()
{
}

VarInt::VarInt(s32 value) : Var<s32>(value)
{
}

ByteBuffer& operator>>(ByteBuffer& buf, VarInt& value)
{
	s32 numRead = 0;
	s32 result = 0;
	u8 read;
	do 
	{
		try 
		{
			read = buf.Read<u8>();
		}
		catch (std::runtime_error e)
		{
			throw std::runtime_error("VarInt too small");
		}

		u8 value = read & 0b01111111;
		result |= value << (7 * numRead);

		numRead++;
		if (numRead > 5) 
		{
			throw std::runtime_error("VarInt is too long");
		}
	} while ((read & 0b10000000) != 0);
	
	value.SetValue(result);

	return buf;
}

ByteBuffer& operator<<(ByteBuffer& buf, VarInt&& value)
{
	return buf << value;
}

ByteBuffer& operator<<(ByteBuffer& buf, const VarInt& value)
{
	u32 tmp = value.Value();
	do
	{
		u8 temp = tmp & 0b01111111;
		tmp >>= 7;
		// Last bit denotes if there are more bytes
		if (tmp != 0)
		{
			temp |= 0b10000000;
		}
		buf.Write(temp);
	} while (tmp != 0);

	return buf;
}

u8 VarInt::Size()
{
	u32 tmp = m_Value;
	u8 size = 0;
	do
	{
		tmp >>= 7;
		size++;
	} while (tmp != 0);
	return size;
}

template<>
VarInt ByteBuffer::Read()
{
	VarInt res;
	*this >> res;
	return res;
}

template<>
void ByteBuffer::Write(const VarInt val)
{
	*this << val;
}


}
#pragma once

#include <common/Typedef.hpp>
#include <networking/types/Var.hpp>
#include <networking/ByteBuffer.hpp>

namespace mcidle {

class VarInt : public Var<s32>
{
public:
	VarInt();
	VarInt(s32);

	// The size of the VarInt in bytes
	u8 Size();
};

template<>
VarInt ByteBuffer::Read();
template<>
void ByteBuffer::Write(const VarInt);

ByteBuffer& operator<<(ByteBuffer&, VarInt&&);
ByteBuffer& operator<<(ByteBuffer&, const VarInt&);
ByteBuffer& operator>>(ByteBuffer&, const VarInt&);

}

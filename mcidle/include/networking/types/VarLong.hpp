#pragma once

#include <common/Typedef.hpp>
#include <networking/ByteBuffer.hpp>
#include <networking/types/Var.hpp>

namespace mcidle
{

class VarLong : public Var<s64>
{
  public:
	VarLong();
	VarLong(s64);
};

ByteBuffer & operator<<(ByteBuffer &, VarLong &&);
ByteBuffer & operator<<(ByteBuffer &, const VarLong &);
ByteBuffer & operator>>(ByteBuffer &, VarLong &);

}  // namespace mcidle

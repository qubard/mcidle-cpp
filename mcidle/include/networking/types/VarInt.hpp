#pragma once

#include <common/Typedef.hpp>
#include <networking/types/Var.hpp>

namespace mcidle
{

class VarInt : public Var<s32>
{
  public:
	VarInt();
	VarInt(s32);

	// The size of the VarInt in bytes
	u8 Size();
};

}  // namespace mcidle

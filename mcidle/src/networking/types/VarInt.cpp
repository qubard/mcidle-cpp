#include <networking/types/VarInt.hpp>

namespace mcidle {

VarInt::VarInt()
    : Var<s32>()
{
}

VarInt::VarInt(s32 value)
    : Var<s32>(value)
{
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

}  // namespace mcidle

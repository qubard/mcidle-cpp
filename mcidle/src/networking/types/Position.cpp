#include <networking/types/Position.hpp>

namespace mcidle {

// This code is very version specific!!!
ByteBuffer& operator>>(ByteBuffer& buf, Position& loc)
{
    u64 pos;
    buf >> pos;
    s64 x = pos >> 38;
    s64 y = (pos >> 26) & 0xFFF;
    s64 z = pos & 0x3FFFFFF;

    if (x >= 2 << 24) x -= 2 << 25;
    if (y >= 2 << 11) y -= 2 << 10;
    if (z >= 2 << 24) z -= 2 << 25;

    loc.X = x;
    loc.Y = y;
    loc.Z = z;

    return buf;
}

ByteBuffer& operator<<(ByteBuffer& buf, Position& loc)
{
    s64 pos = ((loc.X & 0x3FFFFFF) << 38) | ((loc.Y & 0xFFF) << 26) | (loc.Z & 0x3FFFFFF);
    buf << pos;

    return buf;
}

} // ns mcidle

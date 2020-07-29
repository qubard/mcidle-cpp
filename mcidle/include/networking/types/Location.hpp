#pragma once 

namespace mcidle {

struct Location {
    s64 X;
    s64 Y;
    s64 Z;
};

ByteBuffer& operator>>(ByteBuffer& buf, Location& loc)
{
    s64 pos;
    buf >> pos;
    s32 x = pos >> 38;
    s32 y = (pos >> 26) & 0xFFF;
    s32 z = pos & 0x3FFFFFF;

    if (x >= 2 << 25) x -= 2 << 26;
    if (y >= 2 << 11) y -= 2 << 12;
    if (z >= 2 << 25) z -= 2 << 26;

    loc.X = x;
    loc.Y = y;
    loc.Z = z;

    return buf;
}

ByteBuffer& operator<<(ByteBuffer& buf, Location& loc)
{
    s64 pos = ((loc.X & 0x3FFFFFF) << 38) | ((loc.Y & 0xFFF) << 26) | (loc.Z & 0x3FFFFFF);
    buf << pos;

    return buf;
}



} // ns mcidle

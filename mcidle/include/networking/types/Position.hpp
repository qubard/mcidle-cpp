#pragma once 

#include <networking/ByteBuffer.hpp>

namespace mcidle {

struct Position {
    s64 X;
    s64 Y;
    s64 Z;
};

ByteBuffer& operator>>(ByteBuffer&, Position&);
ByteBuffer& operator<<(ByteBuffer&, Position&);

} // ns mcidle

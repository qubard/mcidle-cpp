#pragma once 

#include <networking/ByteBuffer.hpp>

namespace mcidle {

struct Location {
    s64 X;
    s64 Y;
    s64 Z;
};

ByteBuffer& operator>>(ByteBuffer&, Location&);
ByteBuffer& operator<<(ByteBuffer&, Location&);

} // ns mcidle

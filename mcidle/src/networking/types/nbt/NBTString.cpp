#include <networking/types/nbt/NBTString.hpp>

namespace mcidle {
namespace nbt {

ByteBuffer& operator<<(ByteBuffer& buf, NBTString& str)
{
    auto len = str.Value().size();
    buf << static_cast<u16>(len);
    buf.Write((u8*)str.Value().c_str(), len);
    return buf;
}

ByteBuffer& operator<<(ByteBuffer& buf, NBTString&& str)
{
    printf("called move semantics on nbtstring&&\n");
    buf << str;
    return buf;
}

} // ns mcidle
} // ns nbt

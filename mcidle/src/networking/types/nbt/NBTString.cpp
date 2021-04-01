#include <networking/types/nbt/NBTString.hpp>

namespace mcidle {
namespace nbt {

    ByteBuffer &operator<<(ByteBuffer &buf, NBTString &str)
    {
        auto len = str.Value().size();
        buf << static_cast<u16>(len);
        buf.Write((u8 *)str.Value().c_str(), len);
        return buf;
    }

    // Read an NBTString (prefixed with 2 byte len)
    ByteBuffer &operator>>(ByteBuffer &buf, NBTString &str)
    {
        u16 len = buf.Read<u16>();
        std::string tmp;
        tmp.resize(len);
        buf.Read((u8 *)tmp.c_str(), len);
        str.SetValue(tmp);
        return buf;
    }

    ByteBuffer &operator<<(ByteBuffer &buf, NBTString &&str)
    {
        printf("called move semantics on nbtstring&&\n");
        buf << str;
        return buf;
    }

}  // namespace nbt
}  // namespace mcidle

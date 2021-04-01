#include <networking/types/VarLong.hpp>

namespace mcidle {

VarLong::VarLong()
    : Var<s64>()
{
}

VarLong::VarLong(s64 value)
    : Var<s64>(value)
{
}

ByteBuffer &operator>>(ByteBuffer &buf, VarLong &value)
{
    u32 numRead = 0;
    s64 result = 0;
    u8 read;
    do
    {
        read = buf.Read<u8>();
        u64 value = read & 0b01111111;
        result |= value << (7 * numRead);

        numRead++;
        if (numRead > 10)
        {
            throw std::runtime_error("VarLong is too long");
        }
    } while ((read & 0b10000000) != 0);

    value.SetValue(result);

    return buf;
}

ByteBuffer &operator<<(ByteBuffer &buf, VarLong &&value)
{
    return buf << const_cast<const VarLong &>(value);
}

ByteBuffer &operator<<(ByteBuffer &buf, const VarLong &value)
{
    // Conversion from signed to unsigned fixes a bug
    // where with negative integers the sign bit gets
    // shifted in
    u64 val = value.Value();
    do
    {
        u8 temp = val & 0b01111111;
        val = val >> 7;
        if (val != 0)
        {
            temp |= 0b10000000;
        }
        buf.Write(temp);
    } while (val != 0);
    return buf;
}

}  // namespace mcidle
#pragma once

#include <networking/types/nbt/NBTString.hpp>
#include <networking/types/nbt/Tag.hpp>

#include <iostream>

namespace mcidle {
namespace nbt {

// An atomic POD tag (float, double, int, ..)
// This could be more descriptive, since this tag
// has a label explicitly
template <typename T>
class TagPod : public Tag {
public:
    void SetValue(T value)
    {
        m_Value = value;
    }

    void Serialize(ByteBuffer& buf)
    {
        printf("TagPod serialize\n");
        WriteType(buf);
        auto name = NBTString(m_Name);
        buf << name;
        std::cout << "TagPod name is " << m_Name << "!\n";
        buf << m_Value;
    }

    T& Value()
    {
        return m_Value;
    }
private:
    template<class Q=T, typename std::enable_if<std::is_same<Q, s8>::value>::type* = nullptr>
    void WriteType(ByteBuffer& buf)
    {
        buf << (s8)1;
    }

    template<class Q=T, typename std::enable_if<std::is_same<Q, s32>::value>::type* = nullptr>
    void WriteType(ByteBuffer& buf)
    {
        buf << (s8)3;
    }

    template<class Q=T, typename std::enable_if<std::is_same<Q, s16>::value>::type* = nullptr>
    void WriteType(ByteBuffer& buf)
    {
        buf << (s8)2;
    }
    
    template<class Q=T, typename std::enable_if<std::is_same<Q, NBTString>::value>::type* = nullptr>
    void WriteType(ByteBuffer& buf)
    {
        buf << (s8)8;
    }

    template<class Q=T, typename std::enable_if<std::is_same<Q, double>::value>::type* = nullptr>
    void WriteType(ByteBuffer& buf)
    {
        buf << (s8)6;
    }

    template<class Q=T, typename std::enable_if<std::is_same<Q, float>::value>::type* = nullptr>
    void WriteType(ByteBuffer& buf)
    {
        buf << (s8)5;
    }

    T m_Value;
};

template <typename T>
ByteBuffer& operator>>(ByteBuffer& buf, TagPod<T>& value)
{
    NBTString name;
    buf >> name;
    T val;
    buf >> val;
    // Label the tag pod
    value.SetName(name.Value());
    value.SetValue(val);
    return buf;
}

template <typename T>
ByteBuffer& operator<<(ByteBuffer& buf, TagPod<T>& value)
{
    value.Serialize(buf);
    return buf;
}

} // ns nbt
} // ns mcidle

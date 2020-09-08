#pragma once

#include <string>
#include <common/Typedef.hpp>
#include <networking/ByteBuffer.hpp>

namespace mcidle {
namespace nbt {

class NBTString {
public:
    NBTString() {};
    NBTString(std::string& str) : m_Value(str)
    {
    }

    void SetValue(std::string& str)
    {
        m_Value = std::move(str);
    }

    std::string& Value()
    {
        return m_Value;
    }
private:
    std::string m_Value;
};

ByteBuffer& operator<<(ByteBuffer&, NBTString&);
ByteBuffer& operator<<(ByteBuffer&, NBTString&&);

} // ns mcidle
} // ns nbt

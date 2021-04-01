#pragma once

#include <common/Typedef.hpp>
#include <networking/ByteBuffer.hpp>
#include <string>

namespace mcidle {
namespace nbt {

    class NBTString
    {
    public:
        NBTString(){};
        NBTString(std::string &str)
            : m_Value(str)
        {
        }

        void SetValue(std::string &str)
        {
            m_Value = std::move(str);
        }

        std::string &Value()
        {
            return m_Value;
        }

    private:
        std::string m_Value;
    };

    ByteBuffer &operator>>(ByteBuffer &, NBTString &);

    ByteBuffer &operator<<(ByteBuffer &, NBTString &);
    ByteBuffer &operator<<(ByteBuffer &, NBTString &&);

}  // namespace nbt
}  // namespace mcidle

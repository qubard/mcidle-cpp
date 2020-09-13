#pragma once

#include <networking/ByteBuffer.hpp>
#include <string>

namespace mcidle {
namespace nbt {

class Tag {
public:
    void SetName(std::string& name)
    {
        m_Name = std::move(name);
    }

    virtual void Serialize(ByteBuffer&);

    std::string& Name()
    {
        return m_Name;
    }
protected:
    std::string m_Name;
};

} // ns mcidle 
} // ns nbt

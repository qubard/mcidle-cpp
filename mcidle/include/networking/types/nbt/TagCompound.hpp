#pragma once

#include <networking/types/nbt/NBTString.hpp>
#include <networking/types/nbt/Tag.hpp>
#include <networking/types/nbt/TagListFwd.hpp>

namespace mcidle {
namespace nbt {

// A list of tags (the only root tag type)
class TagCompound : public Tag {
public:
    TagCompound();

    void Push(std::shared_ptr<Tag> tag);

    void Serialize(ByteBuffer& buf);

    std::vector<std::shared_ptr<Tag>>& Tags();
    bool Empty() const;
    std::size_t Size() const ;
private:
    std::vector<std::shared_ptr<Tag>> m_Tags;
};

} // ns nbt
} // ns mcidle

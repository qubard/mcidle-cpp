#include <networking/types/nbt/TagCompound.hpp>
#include <networking/types/nbt/TagList.hpp>

namespace mcidle {
namespace nbt {

TagCompound::TagCompound()
{
    m_Tags = std::vector<std::shared_ptr<Tag>>();
}

void TagCompound::Push(std::shared_ptr<Tag> tag)
{
    m_Tags.push_back(tag);
}

void TagCompound::Serialize(ByteBuffer& buf) 
{
    printf("Called tag compound serialize\n");
}

std::vector<std::shared_ptr<Tag>>& TagCompound::Tags()
{
    return m_Tags;
}

bool TagCompound::Empty() const
{
    return m_Tags.size() == 0;
}

std::size_t TagCompound::Size() const 
{
    return m_Tags.size();
}

} // ns mcidle 
} // ns nbt

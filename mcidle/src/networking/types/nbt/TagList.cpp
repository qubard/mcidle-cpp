#include <networking/types/nbt/TagList.hpp>
#include <networking/types/nbt/TagCompound.hpp>

namespace mcidle {
namespace nbt {

void TagList::Push(s8 value)
{
    m_Bytes.push_back(value);
}

void TagList::Push(NBTString& value)
{
    m_Strings.push_back(value);
}

void TagList::Push(VarInt& value)
{
    m_Ints.push_back(value);
}

void TagList::Push(VarLong& value)
{
    m_Longs.push_back(value);
}

void TagList::Push(TagCompound value)
{
    m_Tags.push_back(value);
}

u32 TagList::Size() const
{
    return m_Bytes.size() + m_Strings.size() + m_Longs.size() + m_Ints.size() + m_Tags.size();
}

void TagList::Serialize(ByteBuffer& buf) 
{
    printf("Called list serialize\n");
    buf << TAG_LIST;
    buf << NBTString(m_Name);
    std::cout << "List name is :" << m_Name << "\n";
    buf << m_ListType;

    s32 size = Size();
    buf << size;
    printf("Sending size.. %d %d %d %d\n", m_Bytes.size(), m_Strings.size(), m_Ints.size(), m_Tags.size());

    if (m_ListType == TAG_BYTE) {
        printf("Writing bytes..\n");
        for (auto& tag: m_Bytes)
            buf << tag;
    } else if (m_ListType == TAG_STRING) {
        printf("Writing tag string..\n");
        for (auto& tag: m_Strings)
            buf << tag;
    } else if (m_ListType == TAG_LONG) {
        printf("Writing tag long..\n");
        for (auto& tag: m_Longs)
            buf << tag;
    } else if (m_ListType == TAG_INT) {
        printf("Writing tag int..\n");
        for (auto& tag: m_Ints)
            buf << tag;
    } else if (m_ListType == TAG_COMPOUND) {
        printf("Writing tag compound..\n");
        for (TagCompound& tag: m_Tags) {
            printf("writing compound %d!!!!!\n", tag.Size());
            SerializeTagCompoundInner(buf, tag);
            printf("aaa...\n");
        }
        printf("TTTTTTTTTT\n");
    } else {
        throw std::runtime_error("Unimplemented list type during serialization"); 
    }
}

TagType TagList::ListType() const
{
    return m_ListType;
}

void TagList::SetListType(TagType type)
{
    m_ListType = type;
}

ByteBuffer& operator>>(ByteBuffer& buf, TagList& value)
{
    NBTString name;
    buf >> name;
    value.SetName(name.Value());

    TagType listType;
    buf >> listType;
    value.SetListType(listType);

    if (listType == TAG_COMPOUND)
    {
        s32 len;
        buf >> len;
        while (len > 0)
        {
            TagCompound tag;
            DeserializeTagCompoundInner(buf, tag);   
            len--;
            value.Push(tag);
        }
    } 
    else
    {
        throw std::runtime_error("unimplemented deserialize taglist");
    }

    return buf;
}



} // ns mcidle 
} // ns nbt

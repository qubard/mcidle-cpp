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
    buf << *this;
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

// Deserialize a tag compound without reading the name first
void DeserializeTagCompoundInner(ByteBuffer& buf, TagCompound& value)
{
    TagType type = TAG_END;
    do
    {
        buf >> type;
        if (type == TAG_SHORT)
        {
            auto tag = std::make_shared<TagShort>();
            buf >> *tag;
            value.Push(tag);
        }
        else if (type == TAG_BYTE)
        {
            auto tag = std::make_shared<TagByte>();
            buf >> *tag;
            value.Push(tag);
        }
        else if (type == TAG_INT)
        {
            auto tag = std::make_shared<TagInt>();
            buf >> *tag;
            value.Push(tag);
        }
        else if (type == TAG_FLOAT)
        {
            auto tag = std::make_shared<TagFloat>();
            buf >> *tag;
            value.Push(tag);
        }
        else if (type == TAG_DOUBLE)
        {
            auto tag = std::make_shared<TagDouble>();
            buf >> *tag;
            value.Push(tag);
        } else if (type == TAG_BYTE_ARRAY)
        {
            /*TagList lis;
            lis.SetListType(TAG_BYTE);
            buf >> lis;
            value.Push(lis);*/
        } else if (type == TAG_STRING)
        {
            auto tag = std::make_shared<TagString>();
            buf >> *tag;
            value.Push(tag);
        } else if (type == TAG_LIST)
        {
            printf("Pushing list lmfao!!\n");
            NBTString name;
            buf >> name;
            TagType listType;
            buf >> listType;

            printf("Got list type :%d\n", listType);

            auto lis = std::make_shared<TagList>();
            lis->SetListType(listType);
            lis->SetName(name.Value());

            std::cout << "Got list with name " << name.Value() << "\n";

            if (listType == TAG_COMPOUND)
            {
                s32 len;
                buf >> len;
                printf("Got list with length: %d\n", len);
                while (len > 0)
                {
                    TagCompound tag;
                    DeserializeTagCompoundInner(buf, tag);
                    len--;
                    lis->Push(tag);
                }

                value.Push(lis);
            } 
            else
            {
                throw std::runtime_error("unimplemented list type");
            }
        } else if (type == TAG_COMPOUND)
        {
            auto tag = std::make_shared<TagCompound>();
            DeserializeTagCompound(buf, *tag);
            value.Push(tag);
        } else if (type == TAG_INT_ARRAY)
        {
            throw std::runtime_error("unimplemented tag_int_array");
            /*TagList lis;
            lis.SetListType(TAG_INT);
            buf >> lis;
            value.Push(lis);*/
        } else if (type == TAG_LONG_ARRAY)
        {
            throw std::runtime_error("unimplemented tag_long_array");
            /*TagList lis;
            lis.SetListType(TAG_LONG);
            buf >> lis;
            value.Push(lis);*/
        } else if (type == TAG_END) {
            break;
        } else {
            printf("no type: %d\n", type);
        }

        printf("Pushed list size.. %d\n", value.Size());
    } while (type != TAG_END);
}

// Deserialize a tag compound with the name
void DeserializeTagCompound(ByteBuffer& buf, TagCompound& value)
{
    NBTString name;
    buf >> name;
    value.SetName(name.Value());
    DeserializeTagCompoundInner(buf, value);
    printf("done %d\n", value.Size());
}

void SerializeTagCompoundInner(ByteBuffer& buf, TagCompound& value)
{
    TagType type = TAG_END;

    printf("Serialize tags size: %d\n", value.Tags().size());

    for (auto& tag : value.Tags())
    {
        tag->Serialize(buf);
    }

    buf << TAG_END;
}

void SerializeTagCompound(ByteBuffer& buf, TagCompound& value)
{
    buf << NBTString(value.Name());
    SerializeTagCompoundInner(buf, value);
}

// Deserialize a root tag compound type
ByteBuffer& operator>>(ByteBuffer& buf, TagCompound& value)
{
    TagType type;
    buf >> type;

    if (type == TAG_END) 
    {
        return buf;
    }

    // For some reason this gets hit sometimes
    if (type != TAG_COMPOUND) 
        return buf;

    DeserializeTagCompound(buf, value);

	return buf;
}

ByteBuffer& operator<<(ByteBuffer& buf, TagCompound& value)
{
    if (value.Empty())
    {
        buf << TAG_END;
        return buf;
    }

    buf << TAG_COMPOUND;

    SerializeTagCompound(buf, value);

    return buf;
}

} // ns mcidle 
} // ns nbt

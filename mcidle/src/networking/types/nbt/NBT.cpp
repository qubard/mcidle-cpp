#include <networking/types/nbt/NBT.hpp>

#include <iostream>

namespace mcidle {
namespace nbt {

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

// Read an NBTString (prefixed with 2 byte len)
ByteBuffer& operator>>(ByteBuffer& buf, NBTString& str)
{
	u16 len = buf.Read<u16>();
	std::string tmp;
	tmp.resize(len);
	buf.Read((u8*)tmp.c_str(), len);
    str.SetValue(tmp);
    return buf;
}

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
            } 
            else
            {
                throw std::runtime_error("unimplemented list type");
            }

            value.Push(lis);
        } else if (type == TAG_COMPOUND)
        {
            auto tag = std::make_shared<TagCompound>();
            DeserializeTagCompound(buf, *tag);
            value.Push(tag);
        } else if (type == TAG_INT_ARRAY)
        {
            /*TagList lis;
            lis.SetListType(TAG_INT);
            buf >> lis;
            value.Push(lis);*/
        } else if (type == TAG_LONG_ARRAY)
        {
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
        printf("called serialize inner\n");
        tag->Serialize(buf);
        printf("done serialize inner\n");
    }

    buf << TAG_END;

    /*
    do
    {
        buf >> type;

        printf("type:%d\n", type);
        if (type == TAG_SHORT)
        {
            TagShort tag;
            buf >> tag;
            value.Push(tag);
        }
        else if (type == TAG_BYTE)
        {
            TagByte tag;
            buf >> tag;
            value.Push(tag);
        }
        else if (type == TAG_INT)
        {
            TagInt tag;
            buf >> tag;
            value.Push(tag);
        }
        else if (type == TAG_FLOAT)
        {
            TagFloat tag;
            buf >> tag;
            value.Push(tag);
        }
        else if (type == TAG_DOUBLE)
        {
            TagDouble tag;
            buf >> tag;
            value.Push(tag);
        } else if (type == TAG_BYTE_ARRAY)
        {
            TagList lis;
            lis.SetListType(TAG_BYTE);
            buf >> lis;
            value.Push(lis);
        } else if (type == TAG_STRING)
        {
            TagString tag;
            buf >> tag;
            value.Push(tag);
        } else if (type == TAG_LIST)
        {
            TagList lis;
            buf >> lis;
            value.Push(lis);
        } else if (type == TAG_COMPOUND)
        {
            TagCompound tag;
            DeserializeTagCompound(buf, tag);
            value.Push(tag);
        } else if (type == TAG_INT_ARRAY)
        {
            TagList lis;
            lis.SetListType(TAG_INT);
            buf >> lis;
            value.Push(lis);
        } else if (type == TAG_LONG_ARRAY)
        {
            TagList lis;
            lis.SetListType(TAG_LONG);
            buf >> lis;
            value.Push(lis);
        }
    } while (type != TAG_END);*/
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

    printf("Started >>..\n");

    if (type == TAG_END) 
    {
        printf("Tag end\n");
        return buf;
    }

    printf("Got type: %d\n", type);

    // For some reason this gets hit sometimes
    if (type != TAG_COMPOUND) 
        return buf;

    printf("Deserializing tag compound\n");

    DeserializeTagCompound(buf, value);

    printf("o: %d\n", value.Size());

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

} // ns nbt
} // ns mcidle

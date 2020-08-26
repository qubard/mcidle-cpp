#include <networking/types/nbt/NBT.hpp>

#include <iostream>

namespace mcidle {
namespace nbt {

template <typename T>
ByteBuffer& operator>>(ByteBuffer& buf, TagList<T>& value)
{
    s32 len;
    buf >> len;
    while (len > 0)
    {
        T val;
        buf >> val;
        value.Push(val);
        len--;
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
    T val;
    printf("Reading name..\n");
    NBTString name;
    buf >> name;
    std::cout << "TagPod name: " << name.Value() << "\n";
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
            TagByteArray tag;
            buf >> tag;
            value.PushList(tag);
        } else if (type == TAG_STRING)
        {
            TagString tag;
            buf >> tag;
            value.Push(tag);
        } else if (type == TAG_LIST)
        {
            NBTString name;
            buf >> name;
            TagType listType;
            buf >> listType;

            if (listType == TAG_COMPOUND)
            {
                s32 len;
                buf >> len;
                printf("got len: %d\n", len);
                {
                    DeserializeTagCompoundInner(buf, value);   
                    len--;
                }
                printf("fin\n");
                std::cout << buf.Hex() << "\n";
            } 
            else
            {
                throw std::runtime_error("unimplemented");
            }
        } else if (type == TAG_COMPOUND)
        {
            TagCompound tag;
            DeserializeTagCompound(buf, tag);
            value.PushCompound(tag);
        } else if (type == TAG_INT_ARRAY)
        {
            TagIntArray tag;
            buf >> tag;
            value.PushList(tag);
        } else if (type == TAG_LONG_ARRAY)
        {
            TagLongArray tag;
            buf >> tag;
            value.PushList(tag);
        }
    } while (type != TAG_END);
}

// Deserialize a tag compound with the name
void DeserializeTagCompound(ByteBuffer& buf, TagCompound& value)
{
    NBTString name;
    buf >> name;
    DeserializeTagCompoundInner(buf, value);
}

// Deserialize a root tag compound type
ByteBuffer& operator>>(ByteBuffer& buf, TagCompound& value)
{
    TagType type;
    buf >> type;

    if (type == TAG_END) 
        return buf;

    if (type != TAG_COMPOUND) 
        throw std::runtime_error("Expected tag compound for root type");

    DeserializeTagCompound(buf, value);

	return buf;
}

}
}

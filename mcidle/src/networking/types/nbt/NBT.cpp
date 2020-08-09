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
    NBTString name;
    buf >> name;
    std::cout << "TagPod name: " << name.Value() << "\n";
    buf >> val;
    // Label the tag pod
    value.SetName(name.Value());
    value.SetValue(val);
    return buf;
}

ByteBuffer& operator>>(ByteBuffer& buf, TagCompound& value)
{
    TagType type;
    NBTString name;
    buf >> name;
    std::cout << "name of tag compound: " << name.Value() << "\n";
    printf("Trying to decode tag compound\n");
    do
    {
        buf >> type;
        printf("Type: %d\n", type);

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
            printf("Value: %d\n", tag.Value());
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
            std::cout << "Tag string: (" << tag.Name() << " : " << tag.Value().Value() << ")\n";
        } else if (type == TAG_LIST)
        {
            NBTString name;
            buf >> name;
            TagType listType;
            buf >> listType;
            printf("Got tag list type: %d\n", listType);
            throw std::runtime_error("unimplemented");
        } else if (type == TAG_COMPOUND)
        {
            TagCompound tag;
            buf >> tag;
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

    printf("------------------\n");

	return buf;
}

}
}

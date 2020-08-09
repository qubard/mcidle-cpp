#pragma once

#include <networking/types/VarInt.hpp>
#include <networking/types/VarLong.hpp>
#include <iostream>

namespace mcidle {
namespace nbt {

class Tag {};

typedef u8 TagType;

// Tag id types
const TagType TAG_END = 0;
const TagType TAG_BYTE = 1;
const TagType TAG_SHORT = 2;
const TagType TAG_INT = 3;
const TagType TAG_LONG = 4;
const TagType TAG_FLOAT = 5;
const TagType TAG_DOUBLE = 6;
const TagType TAG_BYTE_ARRAY = 7;
const TagType TAG_STRING = 8;
const TagType TAG_LIST = 9;
const TagType TAG_COMPOUND = 10;
const TagType TAG_INT_ARRAY = 11;
const TagType TAG_LONG_ARRAY = 12;

// An atomic POD tag (float, double, int, ..)
// This could be more descriptive, since this tag
// has a label explicitly
template <typename T>
class TagPod : public Tag {
public:
    void SetValue(T value)
    {
        m_Value = value;
    }

    void SetName(std::string& name)
    {
        m_Name = std::move(name);
    }

    std::string& Name()
    {
        return m_Name;
    }

    T& Value()
    {
        return m_Value;
    }
private:
    std::string m_Name;
    T m_Value;
};

template <typename T>
class TagList : public Tag {
public:
    void Push(T value)
    {
        m_Values.push_back(value);
    }

    std::vector<T> m_Values;
};

// A list of tags (the only root tag type)
class TagCompound : public Tag {
public:
    TagCompound()
    {
        m_Tags = std::vector<Tag>();
    }

    void PushCompound(TagCompound& tag)
    {
        m_Tags.push_back(tag);
    }

    template <typename T>
    void Push(TagPod<T> tag)
    {
        m_Tags.push_back(tag);
    }

    template <typename T>
    void PushList(TagList<T> tag)
    {
        m_Tags.push_back(tag);
    }

    std::vector<Tag> m_Tags;
};

class NBTString {
public:
    NBTString() {};
    NBTString(std::string& str) : m_Value(std::move(str))
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

using TagByte = TagPod<s8>;
using TagShort = TagPod<s16>;
using TagInt = TagPod<s32>;
using TagLong = TagPod<s64>;
using TagFloat = TagPod<float>;
using TagDouble = TagPod<double>;
using TagByteArray = TagList<s8>;
using TagString = TagPod<NBTString>;
using TagIntArray = TagList<VarInt>;
using TagLongArray = TagList<VarLong>;

template <typename T>
ByteBuffer& operator>>(ByteBuffer&, TagList<T>&);
// Read an NBTString (prefixed with 2 byte len)
ByteBuffer& operator>>(ByteBuffer&, NBTString&);

template <typename T>
ByteBuffer& operator>>(ByteBuffer&, TagPod<T>&);
ByteBuffer& operator>>(ByteBuffer&, TagCompound&);

}
}

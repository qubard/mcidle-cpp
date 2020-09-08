#pragma once

#include <networking/types/nbt/TagPod.hpp>

namespace mcidle {
namespace nbt {

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

using TagByte = TagPod<s8>;
using TagShort = TagPod<s16>;
using TagInt = TagPod<s32>;
using TagLong = TagPod<s64>;
using TagFloat = TagPod<float>;
using TagDouble = TagPod<double>;
//using TagByteArray = TagList<s8>;
using TagString = TagPod<NBTString>;
//using TagIntArray = TagList<VarInt>;
//using TagLongArray = TagList<VarLong>;

}
}

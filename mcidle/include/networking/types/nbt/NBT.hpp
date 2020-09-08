#pragma once

#include <networking/types/VarInt.hpp>
#include <networking/types/VarLong.hpp>
#include <networking/types/nbt/TagPod.hpp>
#include <networking/types/nbt/NBTString.hpp>
#include <networking/types/nbt/TagCompound.hpp>
#include <networking/types/nbt/TagType.hpp>
#include <networking/types/nbt/Tag.hpp>
#include <networking/types/nbt/TagList.hpp>

#include <iostream>

namespace mcidle {
namespace nbt {

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

// Deserialize routines
void DeserializeTagCompound(ByteBuffer&, TagCompound&);
void DeserializeTagCompoundInner(ByteBuffer&, TagCompound&);

template <typename T>
ByteBuffer& operator>>(ByteBuffer&, TagList&);

// Read an NBTString (prefixed with 2 byte len)
ByteBuffer& operator>>(ByteBuffer&, NBTString&);

template <typename T>
ByteBuffer& operator>>(ByteBuffer&, TagPod<T>&);
ByteBuffer& operator>>(ByteBuffer&, TagCompound&);

// Serialize routines
void SerializeTagCompound(ByteBuffer&, TagCompound&);
void SerializeTagCompoundInner(ByteBuffer&, TagCompound&);

template <typename T>
ByteBuffer& operator<<(ByteBuffer&, TagPod<T>&);
ByteBuffer& operator<<(ByteBuffer&, TagCompound&);

}
}

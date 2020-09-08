#pragma once

#include <networking/types/nbt/NBTString.hpp>
#include <networking/types/VarInt.hpp>
#include <networking/types/VarLong.hpp>
#include <networking/types/nbt/TagType.hpp>
#include <networking/types/nbt/Tag.hpp>
#include <networking/types/nbt/TagCompound.hpp>

namespace mcidle {
namespace nbt {

class TagList : public Tag {
public:
    void Push(s8 value);
    void Push(NBTString& value);
    void Push(VarInt& value);
    void Push(VarLong& value);
    void Push(TagCompound value);
    u32 Size() const;
    void Serialize(ByteBuffer& buf) override;
    TagType ListType() const;
    void SetListType(TagType type);

 private:
    std::vector<s8> m_Bytes;
    std::vector<NBTString> m_Strings;
    std::vector<VarLong> m_Longs;
    std::vector<VarInt> m_Ints;
    std::vector<TagCompound> m_Tags;

    TagType m_ListType;
};

} // ns nbt
} // ns mcidle

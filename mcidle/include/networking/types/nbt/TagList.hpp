#pragma once

#include <networking/types/VarInt.hpp>
#include <networking/types/VarLong.hpp>
#include <networking/types/nbt/NBTString.hpp>
#include <networking/types/nbt/Tag.hpp>
#include <networking/types/nbt/TagCompound.hpp>
#include <networking/types/nbt/TagType.hpp>

namespace mcidle
{
namespace nbt
{

class TagList : public Tag
{
  public:
	void Push(s8);
	void Push(NBTString &);
	void Push(VarInt &);
	void Push(VarLong &);
	void Push(TagCompound);
	u32 Size() const;
	void Serialize(ByteBuffer & buf) override;
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

ByteBuffer & operator>>(ByteBuffer &, TagList &);

}  // namespace nbt
}  // namespace mcidle

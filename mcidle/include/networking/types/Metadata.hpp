#pragma once 

#include <networking/ByteBuffer.hpp>
#include <networking/types/nbt/NBT.hpp>
#include <networking/types/Position.hpp>
#include <networking/types/VarInt.hpp>

namespace mcidle {

/*
template<class ...Ts>
using TupleStruct = boost::tuple<Ts...>;

template <class ...Ts, uint32_t N>
inline void WriteStruct(ByteBuffer& buf, boost::tuple<Ts...>& tup) 
{

}

template <size_t k = 0, class ...Ts, uint32_t N>
inline typename std::enable_if<k < boost::tuples::length<TupleStruct<Ts...>>::value, ByteBuffer&>::type
    WriteStruct(ByteBuffer& buf, TupleStruct<Ts...> & tup) 
{
    auto& val = boost::get<k>(tup);
    buf << val;
    return WriteStruct<k + 1>(buf, tup);
}

template <size_t k, class ...Ts, uint32_t N>
inline typename std::enable_if<k == boost::tuples::length<TupleStruct<Ts...>>::value, ByteBuffer&>::type
    WriteStruct(ByteBuffer& buf, boost::tuple<Ts...>& tup) 
{
    return buf;
}
*/

struct Slot {
    bool Present;
    VarInt ItemID; // if Present
    u8 ItemCount; // if Present
    nbt::TagCompound NBT; // if Present
};

struct OptChat {
    bool Present;
    std::string Chat; // if Present
};

struct Rotation {
    float X;
    float Y;
    float Z;
};

struct OptPosition {
    bool Present;
    Position Pos;
};

struct UUID {
    u64 First;
    u64 Second;
};

struct OptUUID {
    bool Present;
    UUID UUID; // if Present
};

struct OptBlockID {
    bool Present;
    VarInt BlockID;
};

struct VillagerData {
    VarInt Type;
    VarInt Profession;
    VarInt Level;
};

struct OptVarInt {
    bool Present;
    VarInt Value; // if Present
};

struct Particle {
};

ByteBuffer& operator<<(ByteBuffer&, Particle&);
ByteBuffer& operator>>(ByteBuffer&, Particle&);

namespace metavalue {

enum Type {
    Byte,
    VarInt,
    Float,
    String,
    Chat,
    OptChat,
    Slot,
    Boolean,
    Rotation,
    Position,
    OptPosition,
    Direction,
    OptUUID,
    OptBlockID,
    NBT,
    Particle,
    VillagerData,
    OptVarInt,
    Pose
};

} // ns metavalue

struct MetaValue {
    u8 Byte;
    mcidle::VarInt VarInt;
    float Float;
    std::string String;
    std::string Chat;
    OptChat OptChat;
    Slot Slot;
    bool Boolean;
    Rotation Rot;
    Position Pos;
    OptPosition OptPos;
    mcidle::VarInt Direction;
    OptUUID OptUUID;
    OptBlockID OptBlockID;
    nbt::TagCompound NBT;
    Particle Particle;
    VillagerData VillagerData;
    OptVarInt OptVarInt;
    mcidle::VarInt Pose;
};

struct Metadata {
    u8 Index;
    VarInt Type;
    MetaValue Value;
};

void WriteMetaValue(ByteBuffer&, MetaValue&, u8);
void ReadMetaValue(ByteBuffer&, MetaValue&, u8);

ByteBuffer& operator<<(ByteBuffer&, Metadata&);
ByteBuffer& operator>>(ByteBuffer&, Metadata&);

} // ns mcidle

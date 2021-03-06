#pragma once

#include <networking/ByteBuffer.hpp>
#include <networking/types/Position.hpp>
#include <networking/types/VarInt.hpp>
#include <networking/types/nbt/TagCompound.hpp>

namespace mcidle {

/*
template<class ...Ts>
using TupleStruct = boost::tuple<Ts...>;

template <class ...Ts, uint32_t N>
inline void WriteStruct(ByteBuffer& buf, boost::tuple<Ts...>& tup)
{

}

template <size_t k = 0, class ...Ts, uint32_t N>
inline typename std::enable_if<k <
boost::tuples::length<TupleStruct<Ts...>>::value, ByteBuffer&>::type
    WriteStruct(ByteBuffer& buf, TupleStruct<Ts...> & tup)
{
    auto& val = boost::get<k>(tup);
    buf << val;
    return WriteStruct<k + 1>(buf, tup);
}

template <size_t k, class ...Ts, uint32_t N>
inline typename std::enable_if<k ==
boost::tuples::length<TupleStruct<Ts...>>::value, ByteBuffer&>::type
    WriteStruct(ByteBuffer& buf, boost::tuple<Ts...>& tup)
{
    return buf;
}
*/

struct Slot {
    bool Present;          // not in <= 1.13
    VarInt ItemIDInt;      // if Present
    s16 ItemIDShort;       // Sometimes ItemID is a short
    s16 ItemDamage;        // not in >= 1.13
    u8 ItemCount;          // if Present
    nbt::TagCompound NBT;  // if Present
};

struct OptChat {
    bool Present;
    std::string Chat;  // if Present
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
    mcidle::UUID UUID;  // if Present
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

struct EntityData {
    VarInt Id;
    std::vector<s8> UUID;
    VarInt Type;
    double X;
    double Y;
    double Z;
    s64 ServerX;
    s64 ServerY;
    s64 ServerZ;
    u8 Yaw;
    u8 Pitch;
    u8 HeadPitch;
    s16 MotionX;
    s16 MotionY;
    s16 MotionZ;
    std::vector<s8> Meta;
};

struct OptVarInt {
    bool Present;
    VarInt Value;  // if Present
};

struct Particle {
};

ByteBuffer &operator<<(ByteBuffer &, Particle &);
ByteBuffer &operator>>(ByteBuffer &, Particle &);

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

}  // namespace metavalue

struct MetaValue {
    u8 Byte;
    mcidle::VarInt VarInt;
    float Float;
    std::string String;
    std::string Chat;
    mcidle::OptChat OptChat;
    mcidle::Slot Slot;
    bool Boolean;
    Rotation Rot;
    Position Pos;
    OptPosition OptPos;
    mcidle::VarInt Direction;
    mcidle::OptUUID OptUUID;
    mcidle::OptBlockID OptBlockID;
    nbt::TagCompound NBT;
    mcidle::Particle Particle;
    mcidle::VillagerData VillagerData;
    mcidle::OptVarInt OptVarInt;
    mcidle::VarInt Pose;
};

struct Metadata {
    u8 Index;
    VarInt Type;
    MetaValue Value;
};

ByteBuffer &operator<<(ByteBuffer &, Slot &);
ByteBuffer &operator>>(ByteBuffer &, Slot &);

void WriteMetaValue(ByteBuffer &, MetaValue &, u8);
void ReadMetaValue(ByteBuffer &, MetaValue &, u8);

ByteBuffer &operator<<(ByteBuffer &, Metadata &);
ByteBuffer &operator>>(ByteBuffer &, Metadata &);

}  // namespace mcidle

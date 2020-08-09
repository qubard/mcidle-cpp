#include <networking/types/Metadata.hpp>

namespace mcidle {

void WriteMetaValue(ByteBuffer& buf, MetaValue& value, u8 type)
{
    if (type == metavalue::Type::Byte)
    {
        buf << value.Byte;
    } 
    else if (type == metavalue::Type::VarInt)
    {
        buf << value.VarInt;
    }
    else if (type == metavalue::Type::Float)
    {
        buf << value.Float;
    }
    else if (type == metavalue::Type::String)
    {
        buf << value.String;
    }
    else if (type == metavalue::Type::Chat)
    {
        buf << value.Chat;
    }
    else if (type == metavalue::Type::OptChat)
    {
        buf << value.OptChat;
    }
    else if (type == metavalue::Type::Slot)
    {
        buf << value.Slot;
    }
    else if (type == metavalue::Type::Boolean)
    {
        buf << value.Boolean;
    }
    else if (type == metavalue::Type::Rotation)
    {
        buf << value.Rot;
    }
    else if (type == metavalue::Type::Position)
    {
        buf << value.Pos;
    }
    else if (type == metavalue::Type::OptPosition)
    {
        buf << value.OptPos;
    }
    else if (type == metavalue::Type::Direction)
    {
        buf << value.Direction;
    }
    else if (type == metavalue::Type::OptUUID)
    {
        buf << value.OptUUID;
    }
    else if (type == metavalue::Type::OptBlockID)
    {
        buf << value.OptBlockID;
    }
    else if (type == metavalue::Type::NBT)
    {
        buf << value.NBT;
    }
    else if (type == metavalue::Type::Particle)
    {
        buf << value.Particle;
    }
    else if (type == metavalue::Type::VillagerData)
    {
        buf << value.VillagerData;
    }
    else if (type == metavalue::Type::OptVarInt)
    {
        buf << value.OptVarInt;
    }
    else if (type == metavalue::Type::Pose)
    {
        buf << value.Pose;
    }
}

void ReadMetaValue(ByteBuffer& buf, MetaValue& value, u8 type)
{
    printf("Deserializing meta value type : %d\n", type);
    if (type == metavalue::Type::Byte)
    {
        buf >> value.Byte;
        printf("byte: %d\n", value.Byte);
    } 
    else if (type == metavalue::Type::VarInt)
    {
        buf >> value.VarInt;
    }
    else if (type == metavalue::Type::Float)
    {
        buf >> value.Float;
    }
    else if (type == metavalue::Type::String)
    {
        buf >> value.String;
    }
    else if (type == metavalue::Type::Chat)
    {
        buf >> value.Chat;
    }
    else if (type == metavalue::Type::OptChat)
    {
        buf >> value.OptChat;
    }
    else if (type == metavalue::Type::Slot)
    {
        buf >> value.Slot;
    }
    else if (type == metavalue::Type::Boolean)
    {
        buf >> value.Boolean;
    }
    else if (type == metavalue::Type::Rotation)
    {
        buf >> value.Rot;
    }
    else if (type == metavalue::Type::Position)
    {
        buf >> value.Pos;
    }
    else if (type == metavalue::Type::OptPosition)
    {
        buf >> value.OptPos;
    }
    else if (type == metavalue::Type::Direction)
    {
        buf >> value.Direction;
    }
    else if (type == metavalue::Type::OptUUID)
    {
        buf >> value.OptUUID;
    }
    else if (type == metavalue::Type::OptBlockID)
    {
        buf >> value.OptBlockID;
    }
    else if (type == metavalue::Type::NBT)
    {
        buf >> value.NBT;
    }
    else if (type == metavalue::Type::Particle)
    {
        buf >> value.Particle;
    }
    else if (type == metavalue::Type::VillagerData)
    {
        buf >> value.VillagerData;
    }
    else if (type == metavalue::Type::OptVarInt)
    {
        buf >> value.OptVarInt;
    }
    else if (type == metavalue::Type::Pose)
    {
        buf >> value.Pose;
    }
}

ByteBuffer& operator<<(ByteBuffer& buf, Metadata& metadata)
{
    printf("Serializing metadata..\n");
    buf << metadata.Index;
    if (metadata.Index != 0xFF)
    {
        buf << metadata.Type;
        u8 type = metadata.Type.Value();
        WriteMetaValue(buf, metadata.Value, type);
    }
    return buf;
}

ByteBuffer& operator>>(ByteBuffer& buf, Metadata& metadata)
{
    printf("Begun reading meta value\n");
    buf >> metadata.Index;
    if (metadata.Index != 0xFF)
    {
        buf >> metadata.Type;
        u8 type = metadata.Type.Value();
        printf("Type: %d\n", metadata.Type.Value());
        ReadMetaValue(buf, metadata.Value, type);
        printf("Finished reading meta value\n");
    }
    printf("Remaining : %d out of %d\n", buf.ReadOffset(), buf.Size());
    return buf;
}

// Particle is going to have a dummy implementation
// because it's pointless to decode/serialize
// and is a fairly complex structure
ByteBuffer& operator<<(ByteBuffer& buf, Particle&)
{
    buf << VarInt(0);
    buf << VarInt(0);
}

ByteBuffer& operator<<(ByteBuffer& buf, UUID& uuid)
{
    buf << uuid.First;
    buf << uuid.Second; 
}

ByteBuffer& operator>>(ByteBuffer& buf, UUID& uuid)
{
    buf >> uuid.First;
    buf >> uuid.Second;
}

ByteBuffer& operator>>(ByteBuffer& buf, Particle&)
{
    // Don't need to deserialize
}

} // ns mcidle

#include <networking/packet/clientbound/SpawnMob.hpp>

namespace mcidle {
namespace packet {
namespace clientbound {

SpawnMob::SpawnMob()
{
}

SpawnMob::SpawnMob(VarInt EntityID, UUID EntityUUID, VarInt Type, double X, double Y, double Z, s8 Yaw, s8 Pitch, s8 HeadPitch, s16 MotionX, s16 MotionY, s16 MotionZ, Metadata Meta)
    : m_EntityID(EntityID), m_EntityUUID(EntityUUID), m_Type(Type), m_X(X), m_Y(Y), m_Z(Z), m_Yaw(Yaw), m_Pitch(Pitch), m_HeadPitch(HeadPitch), m_MotionX(MotionX), m_MotionY(MotionY), m_MotionZ(MotionZ), m_Meta(Meta)
{
}

void SpawnMob::Mutate(mcidle::game::GameState &state)
{
    printf("Mob with entity id %d spawned at coords: %.2f, %2.f, %.2f\n", m_EntityID.Value(), m_X, m_Y, m_Z);
    auto entity = EntityData{};

    entity.Id = m_EntityID;
    entity.UUID = m_EntityUUID;
    entity.Type = m_Type;
    entity.X = m_X;
    entity.Y = m_Y;
    entity.Z = m_Y;

    entity.Yaw = m_Yaw;
    entity.HeadPitch = m_HeadPitch;
    entity.Pitch = m_Pitch;;

    entity.MotionX = m_MotionX;
    entity.MotionY = m_MotionY;
    entity.MotionZ = m_MotionZ;

    state.LoadEntity(entity);
}

Packet& SpawnMob::Serialize()
{
    *m_FieldBuf << m_EntityID;
    *m_FieldBuf << m_EntityUUID;
    *m_FieldBuf << m_Type;
    *m_FieldBuf << m_X;
    *m_FieldBuf << m_Y;
    *m_FieldBuf << m_Z;
    *m_FieldBuf << m_Yaw;
    *m_FieldBuf << m_Pitch;
    *m_FieldBuf << m_HeadPitch;
    *m_FieldBuf << m_MotionX;
    *m_FieldBuf << m_MotionY;
    *m_FieldBuf << m_MotionZ;
    //*m_FieldBuf << m_Meta;
    *m_FieldBuf << (s8)0; // don't write metadata
    return *this;
}

void SpawnMob::Deserialize(ByteBuffer &buf)
{
    printf("Deserializing spawn mob..\n");
    buf >> m_EntityID;
    buf >> m_EntityUUID;
    buf >> m_Type;
    buf >> m_X;
    buf >> m_Y;
    buf >> m_Z;
    buf >> m_Yaw;
    buf >> m_Pitch;
    buf >> m_HeadPitch;
    buf >> m_MotionX;
    buf >> m_MotionY;
    buf >> m_MotionZ;
    //buf >> m_Meta;
}

}  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

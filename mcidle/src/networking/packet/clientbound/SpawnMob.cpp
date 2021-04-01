#include <networking/packet/clientbound/SpawnMob.hpp>

namespace mcidle {
namespace packet {
    namespace clientbound {

        SpawnMob::SpawnMob()
        {
        }

        SpawnMob::SpawnMob(VarInt EntityId, UUID EntityUUID, VarInt Type,
                           double X, double Y, double Z, s8 Yaw, s8 Pitch,
                           s8 HeadPitch, s16 MotionX, s16 MotionY, s16 MotionZ,
                           Metadata Meta)
            : m_EntityId(EntityId)
            , m_EntityUUID(EntityUUID)
            , m_Type(Type)
            , m_X(X)
            , m_Y(Y)
            , m_Z(Z)
            , m_Yaw(Yaw)
            , m_Pitch(Pitch)
            , m_HeadPitch(HeadPitch)
            , m_MotionX(MotionX)
            , m_MotionY(MotionY)
            , m_MotionZ(MotionZ)
        {
        }

        SpawnMob::SpawnMob(EntityData &entity)
        {
            m_EntityId = entity.Id;
            // m_EntityUUID = entity.UUID;
            m_Type = entity.Type;

            m_X = (double)entity.ServerX / (double)4096.0;
            m_Y = (double)entity.ServerY / (double)4096.0;
            m_Z = (double)entity.ServerZ / (double)4096.0;

            m_Yaw = entity.Yaw;
            m_Pitch = entity.Pitch;
            m_HeadPitch = entity.HeadPitch;

            m_MotionX = entity.MotionX;
            m_MotionY = entity.MotionY;
            m_MotionZ = entity.MotionZ;
        }

        void SpawnMob::Mutate(mcidle::game::GameState &state)
        {
            auto entity = EntityData{};

            entity.Id = m_EntityId;
            // entity.UUID = m_EntityUUID;

            entity.Type = m_Type;
            entity.X = m_X;
            entity.Y = m_Y;
            entity.Z = m_Z;

            entity.ServerX = floor(m_X * 4096.0);
            entity.ServerY = floor(m_Y * 4096.0);
            entity.ServerZ = floor(m_Z * 4096.0);

            entity.Yaw = m_Yaw;
            entity.HeadPitch = m_HeadPitch;
            entity.Pitch = m_Pitch;

            entity.MotionX = m_MotionX;
            entity.MotionY = m_MotionY;
            entity.MotionZ = m_MotionZ;

            state.LoadEntity(entity);
        }

        Packet &SpawnMob::Serialize()
        {
            *m_FieldBuf << m_EntityId;

            // Write a random UUID (for now)
            for (auto i = 0; i < 16; i++)
            {
                *m_FieldBuf << (s8)((i + m_EntityId.Value()) & 0xFF);
            }
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

            // We have no way of serializing metadata
            // it's annoying and changes across versions
            // so don't write it
            *m_FieldBuf << (s8)0xFF;
            return *this;
        }

        void SpawnMob::Deserialize(ByteBuffer &buf)
        {
            printf("Deserializing spawn mob..\n");
            buf >> m_EntityId;
            // Read a dummy UUID
            for (auto i = 0; i < 16; i++)
            {
                s8 tmp;
                buf >> tmp;
            }
            // buf >> m_EntityUUID;
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
        }

    }  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

#pragma once

#include <networking/game/GameState.hpp>
#include <networking/packet/Packet.hpp>
#include <networking/types/Metadata.hpp>

namespace mcidle {
namespace packet {
    namespace clientbound {

        class SpawnMob : public Packet
        {
        public:
            SpawnMob();
            SpawnMob(VarInt, UUID, VarInt, double, double, double, s8, s8, s8, s16, s16, s16, Metadata);
            SpawnMob(EntityData &);

            void Mutate(mcidle::game::GameState &) override;

            Packet &Serialize() override;
            void Deserialize(ByteBuffer &) override;

        private:
            VarInt m_EntityId;
            UUID m_EntityUUID;
            VarInt m_Type;
            double m_X;
            double m_Y;
            double m_Z;
            // Angles are u8
            u8 m_Yaw;
            u8 m_Pitch;
            u8 m_HeadPitch;
            s16 m_MotionX;
            s16 m_MotionY;
            s16 m_MotionZ;
            //Metadata m_Meta;
            std::vector<s8> m_Meta;
        };

    }  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

#include <networking/packet/clientbound/DestroyEntities.hpp>

namespace mcidle {
namespace packet {
    namespace clientbound {

        DestroyEntities::DestroyEntities()
        {
            m_EntityIds = std::vector<s32>();
        }

        DestroyEntities::DestroyEntities(std::vector<s32> &&entityIds)
            : m_EntityIds(std::move(entityIds))
        {
        }

        void DestroyEntities::Mutate(mcidle::game::GameState &state)
        {
            for (auto &id : m_EntityIds)
            {
                state.UnloadEntity(id);
                printf("Unloaded : %d\n", id);
            }
        }

        Packet &DestroyEntities::Serialize()
        {
            *m_FieldBuf << VarInt(m_EntityIds.size());
            for (auto &id : m_EntityIds)
                *m_FieldBuf << VarInt(id);
            return *this;
        }

        void DestroyEntities::Deserialize(ByteBuffer &buf)
        {
            printf("DestroyEntities::Deserialize\n");
            VarInt len;
            buf >> len;
            for (auto idx = 0; idx < len.Value(); idx++)
            {
                VarInt id;
                buf >> id;
                m_EntityIds.push_back(id.Value());
            }
            printf("Got %d entities to remove\n", m_EntityIds.size());
        }

    }  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

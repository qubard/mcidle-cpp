#pragma once

#include <networking/game/GameState.hpp>
#include <networking/packet/Packet.hpp>

namespace mcidle {
namespace packet {
    namespace clientbound {

        class DestroyEntities : public Packet
        {
        public:
            DestroyEntities();
            DestroyEntities(std::vector<s32> &&);

            void Mutate(mcidle::game::GameState &) override;

            Packet &Serialize() override;
            void Deserialize(ByteBuffer &) override;

        private:
            std::vector<s32> m_EntityIds;
        };

    }  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

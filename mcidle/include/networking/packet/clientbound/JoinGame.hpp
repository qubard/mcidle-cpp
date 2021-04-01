#pragma once

#include <networking/packet/Packet.hpp>

namespace mcidle {

namespace packet {
    namespace clientbound {

        class JoinGame : public Packet
        {
        public:
            JoinGame();
            JoinGame(s32, u8, s32, u8, u8, std::string, bool);

            void Mutate(mcidle::game::GameState &) override;

            Packet &Serialize() override;
            void Deserialize(ByteBuffer &) override;

        private:
            s32 m_EntityId;
            u8 m_Gamemode;
            s32 m_Dimension;
            u8 m_Difficulty;
            u8 m_MaxPlayers;
            std::string m_LevelType;
            bool m_DebugInfo;
        };

    }  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

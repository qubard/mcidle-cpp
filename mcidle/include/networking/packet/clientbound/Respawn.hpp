#pragma once

#include <networking/game/GameState.hpp>
#include <networking/packet/Packet.hpp>

/*
Dimension	Int Enum	-1: The Nether, 0: The Overworld, 1: The End
Difficulty	Unsigned Byte	0: Peaceful, 1: Easy, 2: Normal, 3: Hard
Gamemode	Unsigned Byte	0: survival, 1: creative, 2: adventure, 3:
spectator. The hardcore flag is not included Level Type	String (16)	Same as
Join Game
*/

namespace mcidle {

namespace packet {
    namespace clientbound {

        class Respawn : public Packet
        {
        public:
            Respawn();
            Respawn(int, u8, u8, std::string);

            void Mutate(mcidle::game::GameState &) override;

            Packet &Serialize() override;
            void Deserialize(ByteBuffer &) override;

        private:
            s32 m_Dimension;
            u8 m_Difficulty;
            u8 m_Gamemode;
            std::string m_Level;
        };

    }  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

#pragma once

#include <networking/packet/Packet.hpp>

namespace mcidle {

namespace packet {
namespace clientbound {

class JoinGame : public Packet
{
public:
    JoinGame();
    JoinGame(int, u8, int, u8, u8, std::string, bool);

    void Mutate(mcidle::game::GameState &) override;

    Packet &Serialize() override;
    void Deserialize(ByteBuffer &) override;

private:
    int m_EntityId;
    u8 m_Gamemode;
    int m_Dimension;
    u8 m_Difficulty;
    u8 m_MaxPlayers;
    std::string m_LevelType;
    bool m_DebugInfo;
};

}  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

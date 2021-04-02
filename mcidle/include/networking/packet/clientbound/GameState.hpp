#pragma once

#include <networking/game/GameState.hpp>
#include <networking/packet/Packet.hpp>

namespace mcidle
{
namespace packet
{
namespace clientbound
{

const u8 INVALID_BED = 0;
const u8 END_RAINING = 1;
const u8 BEGIN_RAINING = 2;
const u8 CHANGE_GAMEMODE = 3;
const u8 EXIT_END = 4;
const u8 DEMO_MESSAGE = 5;
const u8 ARROW_HITTING_PLAYER = 6;
// The current darkness value
const u8 FADE_VALUE = 7;
// Time in ticks for the sky to fade
const u8 FADE_TIME = 8;

// Plays an elder guardian mob appearance
// effects and sound
const u8 ELDER_GUARDIAN_APPEARANCE = 10;

class GameState : public Packet
{
  public:
	GameState();
	GameState(u8, float);

	void Mutate(mcidle::game::GameState &) override;

	Packet & Serialize() override;
	void Deserialize(ByteBuffer &) override;

  private:
	u8 m_Reason;
	float m_Value;
};

}  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

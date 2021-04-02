#pragma once

#include <networking/game/GameState.hpp>
#include <networking/packet/Packet.hpp>

namespace mcidle
{
namespace packet
{
namespace clientbound
{

class EntityLookRelMove : public Packet
{
  public:
	EntityLookRelMove();
	EntityLookRelMove(VarInt, s16, s16, s16, u8, u8, bool);

	void Mutate(mcidle::game::GameState &) override;

	Packet & Serialize() override;
	void Deserialize(ByteBuffer &) override;

  private:
	VarInt m_EntityID;
	s16 m_DeltaX;
	s16 m_DeltaY;
	s16 m_DeltaZ;
	u8 m_Yaw;
	u8 m_Pitch;
	bool m_OnGround;
};

}  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

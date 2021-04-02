#pragma once

#include <networking/game/GameState.hpp>
#include <networking/packet/Packet.hpp>

namespace mcidle
{
namespace packet
{
namespace clientbound
{

class EntityVelocity : public Packet
{
  public:
	EntityVelocity();
	EntityVelocity(VarInt, s16, s16, s16);

	void Mutate(mcidle::game::GameState &) override;

	Packet & Serialize() override;
	void Deserialize(ByteBuffer &) override;

  private:
	VarInt m_EntityID;
	s16 m_VelX;
	s16 m_VelY;
	s16 m_VelZ;
};

}  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

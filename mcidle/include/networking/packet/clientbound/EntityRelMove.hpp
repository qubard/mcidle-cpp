#pragma once

#include <networking/game/GameState.hpp>
#include <networking/packet/Packet.hpp>

namespace mcidle
{
namespace packet
{
namespace clientbound
{

class EntityRelMove : public Packet
{
  public:
	EntityRelMove();
	EntityRelMove(VarInt, s16, s16, s16, bool);

	void Mutate(mcidle::game::GameState &) override;

	Packet & Serialize() override;
	void Deserialize(ByteBuffer &) override;

  private:
	VarInt m_EntityID;
	s16 m_DeltaX;
	s16 m_DeltaY;
	s16 m_DeltaZ;
	bool m_OnGround;
};

}  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

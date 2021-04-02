#pragma once

#include <networking/game/GameState.hpp>
#include <networking/packet/Packet.hpp>

namespace mcidle
{
namespace packet
{
namespace clientbound
{

class EntityTeleport : public Packet
{
  public:
	EntityTeleport();
	EntityTeleport(VarInt, double, double, double, u8, u8, bool);

	void Mutate(mcidle::game::GameState &) override;

	Packet & Serialize() override;
	void Deserialize(ByteBuffer &) override;

  private:
	VarInt m_EntityID;
	double m_X;
	double m_Y;
	double m_Z;
	u8 m_Yaw;
	u8 m_Pitch;
	bool m_OnGround;
};

}  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

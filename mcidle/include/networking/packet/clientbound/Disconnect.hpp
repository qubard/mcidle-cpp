#pragma once

#include <networking/packet/Packet.hpp>

namespace mcidle
{
namespace packet
{
namespace clientbound
{

class Disconnect : public Packet
{
  public:
	Disconnect();
	Disconnect(std::string);

	virtual std::shared_ptr<Packet> Response(Protocol &, s32) override;

	std::string Reason() const;

	Packet & Serialize() override;
	void Deserialize(ByteBuffer &) override;

  private:
	std::string m_Reason;
};

}  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

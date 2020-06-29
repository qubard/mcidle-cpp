#pragma once

#include <memory>
#include <networking/packet/Packet.hpp>
#include <networking/protocol/Protocol.hpp>

namespace mcidle
{

// 1.12.2 map from id to packet
static ProtocolMap clientboundMap_1_12_2 = {
    {state::LOGIN,
     {{
          0x01,
          []() -> std::unique_ptr<Packet> { return std::make_unique<packet::clientbound::EncryptionRequest>(); },
      },
      {
          0x02,
          []() -> std::unique_ptr<Packet> { return std::make_unique<packet::clientbound::LoginSuccess>(); },
      },
      {
          0x03,
          []() -> std::unique_ptr<Packet> { return std::make_unique<packet::clientbound::SetCompression>(); },
      }}},
    {state::PLAY,
     {{
          0x20,
          []() -> std::unique_ptr<Packet> { return std::make_unique<packet::clientbound::ChunkData>(); },
      },
      {
          0x1F,
          []() -> std::unique_ptr<Packet> { return std::make_unique<packet::clientbound::KeepAlive>(); },
      },
      {
          0x41,
          []() -> std::unique_ptr<Packet> { return std::make_unique<packet::clientbound::UpdateHealth>(); },
      },
      {
          0x35,
          []() -> std::unique_ptr<Packet> { return std::make_unique<packet::clientbound::Respawn>(); },
      },
      {
          0x1E,
          []() -> std::unique_ptr<Packet> { return std::make_unique<packet::clientbound::GameState>(); },
      }}}};

static ProtocolMap serverboundMap_1_12_2 =
{
	{
		state::HANDSHAKE,
		{
			{
				0x00, []() -> std::unique_ptr<Packet> { return std::make_unique<packet::serverbound::Handshake>(); },
			},
		},
	},
	{
		state::LOGIN,
		{
			{
				0x00, []() -> std::unique_ptr<Packet> { return std::make_unique<packet::serverbound::LoginStart>(); },
			},
			{
				0x01, []() -> std::unique_ptr<Packet> { return std::make_unique<packet::serverbound::EncryptionResponse>(); },
			},
		},
	},
};


class Protocol_1_12_2_CB : public Protocol
{
public:
    Protocol_1_12_2_CB(s32);

    s32 PacketId(packet::serverbound::KeepAlive &);
    s32 PacketId(packet::clientbound::UpdateHealth &);
};


class Protocol_1_12_2_SB : public Protocol
{
public:
    Protocol_1_12_2_SB(s32);
};

}

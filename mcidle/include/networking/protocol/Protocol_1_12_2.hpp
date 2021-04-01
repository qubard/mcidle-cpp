#pragma once

#include <memory>
#include <networking/packet/Packet.hpp>
#include <networking/protocol/Protocol.hpp>

namespace mcidle {

// 1.12.2 map from id to packet
static ProtocolMap clientboundMap_1_12_2 = {
    {state::LOGIN,
     {{
          0x01,
          []() -> std::unique_ptr<Packet> {
              return std::make_unique<packet::clientbound::EncryptionRequest>();
          },
      },
      {
          0x00,
          []() -> std::unique_ptr<Packet> {
              return std::make_unique<packet::clientbound::Disconnect>();
          },
      },
      {
          0x02,
          []() -> std::unique_ptr<Packet> {
              return std::make_unique<packet::clientbound::LoginSuccess>();
          },
      },
      {
          0x03,
          []() -> std::unique_ptr<Packet> {
              return std::make_unique<packet::clientbound::SetCompression>();
          },
      }}},
    {state::PLAY,
     {{
          0x20,
          []() -> std::unique_ptr<Packet> {
              return std::make_unique<packet::clientbound::ChunkData>();
          },
      },
      {
          0x1F,
          []() -> std::unique_ptr<Packet> {
              return std::make_unique<packet::clientbound::KeepAlive>();
          },
      },
      {
          0x2F,
          []() -> std::unique_ptr<Packet> {
              return std::make_unique<
                  packet::clientbound::PlayerPositionLook>();
          },
      },

      {
          0x1A,
          []() -> std::unique_ptr<Packet> {
              return std::make_unique<packet::clientbound::Disconnect>();
          },
      },
      {
          0x41,
          []() -> std::unique_ptr<Packet> {
              return std::make_unique<packet::clientbound::UpdateHealth>();
          },
      },
      {
          0x46,
          []() -> std::unique_ptr<Packet> {
              return std::make_unique<packet::clientbound::SpawnPosition>();
          },
      },
      {
          0x1D,
          []() -> std::unique_ptr<Packet> {
              return std::make_unique<packet::clientbound::UnloadChunk>();
          },
      },
      {
          0x10,
          []() -> std::unique_ptr<Packet> {
              return std::make_unique<packet::clientbound::MultiBlockChange>();
          },
      },
      {
          0x0B,
          []() -> std::unique_ptr<Packet> {
              return std::make_unique<packet::clientbound::BlockChange>();
          },
      },
      {
          0x1C,
          []() -> std::unique_ptr<Packet> {
              return std::make_unique<packet::clientbound::Explosion>();
          },
      },
      {
          0x16,
          []() -> std::unique_ptr<Packet> {
              return std::make_unique<packet::clientbound::SetSlot>();
          },
      },
      {
          0x35,
          []() -> std::unique_ptr<Packet> {
              return std::make_unique<packet::clientbound::Respawn>();
          },
      },
      {
          0x23,
          []() -> std::unique_ptr<Packet> {
              return std::make_unique<packet::clientbound::JoinGame>();
          },
      },
      {
          0x03,
          []() -> std::unique_ptr<Packet> {
              return std::make_unique<packet::clientbound::SpawnMob>();
          },
      },
      {
          0x0F,
          []() -> std::unique_ptr<Packet> {
              return std::make_unique<packet::clientbound::ChatMessage>();
          },

      },
      {
          0x32,
          []() -> std::unique_ptr<Packet> {
              return std::make_unique<packet::clientbound::DestroyEntities>();
          },

      },
      {
          0x4C,
          []() -> std::unique_ptr<Packet> {
              return std::make_unique<packet::clientbound::EntityTeleport>();
          },

      },
      {
          0x3E,
          []() -> std::unique_ptr<Packet> {
              return std::make_unique<packet::clientbound::EntityVelocity>();
          },
      },
      {
          0x26,
          []() -> std::unique_ptr<Packet> {
              return std::make_unique<packet::clientbound::EntityRelMove>();
          },

      },
      {
          0x27,
          []() -> std::unique_ptr<Packet> {
              return std::make_unique<packet::clientbound::EntityLookRelMove>();
          },
      },
      {
          0x1E,
          []() -> std::unique_ptr<Packet> {
              return std::make_unique<packet::clientbound::GameState>();
          },
      }}}};

static ProtocolMap serverboundMap_1_12_2 = {
    {
        state::HANDSHAKE,
        {
            {
                0x00,
                []() -> std::unique_ptr<Packet> {
                    return std::make_unique<packet::serverbound::Handshake>();
                },
            },
        },
    },
    {
        state::LOGIN,
        {
            {
                0x00,
                []() -> std::unique_ptr<Packet> {
                    return std::make_unique<packet::serverbound::LoginStart>();
                },
            },
            {
                0x01,
                []() -> std::unique_ptr<Packet> {
                    return std::make_unique<
                        packet::serverbound::EncryptionResponse>();
                },
            },
        },
    },
    {
        state::PLAY,
        {
            {
                0x00,
                []() -> std::unique_ptr<Packet> {
                    return std::make_unique<
                        packet::serverbound::TeleportConfirm>();
                },
            },
        },
    },

};

// Incoming clientbound packets, outbound server packets
class Protocol_1_12_2_CB : public Protocol
{
public:
    Protocol_1_12_2_CB(s32);

    s32 PacketId(packet::serverbound::KeepAlive &) override;
    s32 PacketId(packet::serverbound::Handshake &) override;
    s32 PacketId(packet::serverbound::LoginStart &) override;
    s32 PacketId(packet::serverbound::EncryptionResponse &) override;
};

// Incoming serverbound packets, outbound client packets
class Protocol_1_12_2_SB : public Protocol
{
public:
    Protocol_1_12_2_SB(s32);

    s32 PacketId(packet::clientbound::EntityLookRelMove &) override;
    s32 PacketId(packet::clientbound::UpdateHealth &) override;
    s32 PacketId(packet::clientbound::JoinGame &) override;
    s32 PacketId(packet::clientbound::SpawnPosition &) override;
    s32 PacketId(packet::clientbound::PlayerPositionLook &) override;
    s32 PacketId(packet::clientbound::KeepAlive &) override;
    s32 PacketId(packet::clientbound::ChunkData &) override;
    s32 PacketId(packet::clientbound::SetCompression &) override;
    s32 PacketId(packet::clientbound::LoginSuccess &) override;
    s32 PacketId(packet::clientbound::EncryptionRequest &) override;
    s32 PacketId(packet::clientbound::SetSlot &) override;
    s32 PacketId(packet::clientbound::SpawnMob &) override;
    s32 PacketId(packet::clientbound::DestroyEntities &) override;
    s32 PacketId(packet::clientbound::EntityVelocity &) override;
};

}  // namespace mcidle

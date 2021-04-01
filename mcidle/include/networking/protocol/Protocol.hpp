#pragma once

#include <common/Typedef.hpp>
#include <networking/packet/clientbound/BlockChange.hpp>
#include <networking/packet/clientbound/ChatMessage.hpp>
#include <networking/packet/clientbound/ChunkData.hpp>
#include <networking/packet/clientbound/DestroyEntities.hpp>
#include <networking/packet/clientbound/Disconnect.hpp>
#include <networking/packet/clientbound/EncryptionRequest.hpp>
#include <networking/packet/clientbound/EntityLookRelMove.hpp>
#include <networking/packet/clientbound/EntityRelMove.hpp>
#include <networking/packet/clientbound/EntityTeleport.hpp>
#include <networking/packet/clientbound/EntityVelocity.hpp>
#include <networking/packet/clientbound/Explosion.hpp>
#include <networking/packet/clientbound/GameState.hpp>
#include <networking/packet/clientbound/JoinGame.hpp>
#include <networking/packet/clientbound/KeepAlive.hpp>
#include <networking/packet/clientbound/LoginSuccess.hpp>
#include <networking/packet/clientbound/MultiBlockChange.hpp>
#include <networking/packet/clientbound/PlayerPositionLook.hpp>
#include <networking/packet/clientbound/Respawn.hpp>
#include <networking/packet/clientbound/SetCompression.hpp>
#include <networking/packet/clientbound/SetSlot.hpp>
#include <networking/packet/clientbound/SpawnMob.hpp>
#include <networking/packet/clientbound/SpawnPosition.hpp>
#include <networking/packet/clientbound/UnloadChunk.hpp>
#include <networking/packet/clientbound/UpdateHealth.hpp>
#include <networking/packet/serverbound/EncryptionResponse.hpp>
#include <networking/packet/serverbound/Handshake.hpp>
#include <networking/packet/serverbound/KeepAlive.hpp>
#include <networking/packet/serverbound/LoginStart.hpp>
#include <networking/packet/serverbound/TeleportConfirm.hpp>
#include <unordered_map>

// Packet ids change for different game versions
// When we fwd packets we dont care we just fwd the buffer

// When sending a packet we figure out the packet id
// using a functional override on a type
// e.g virtual s32 PacketId(PackeType&)

// For receiving packets we need a mapping from integer to packet class
// So we do a functional lambda

// This way as long as we implement these mappings for game versions
// we can easily swap them in/out to support different protocol numbers
// and choose the correct protocol accordingly

namespace mcidle {

class Packet;

using PacketFactory = std::unique_ptr<Packet> (*)();

// Inbound packet map
using PacketMap = std::unordered_map<s32, PacketFactory>;

using ProtocolMap = std::unordered_map<s32, PacketMap>;

namespace state {
    const s32 HANDSHAKE = 0x03;
    const s32 PLAY = 0x00;
    const s32 STATUS = 0x01;
    const s32 LOGIN = 0x02;
}  // namespace state

class Protocol
{
public:
    Protocol(s32);
    Protocol(ProtocolMap, s32, s32);

    PacketMap &InboundMap();

    s32 VersionNumber();

    void SetState(s32);

    virtual s32 PacketId(packet::serverbound::EncryptionResponse &);
    virtual s32 PacketId(packet::serverbound::Handshake &);
    virtual s32 PacketId(packet::serverbound::LoginStart &);
    virtual s32 PacketId(packet::serverbound::KeepAlive &);

    virtual s32 PacketId(packet::clientbound::EntityLookRelMove &);
    virtual s32 PacketId(packet::clientbound::EncryptionRequest &);
    virtual s32 PacketId(packet::clientbound::SetCompression &);
    virtual s32 PacketId(packet::clientbound::UpdateHealth &);
    virtual s32 PacketId(packet::clientbound::LoginSuccess &);
    virtual s32 PacketId(packet::clientbound::JoinGame &);
    virtual s32 PacketId(packet::clientbound::SpawnPosition &);
    virtual s32 PacketId(packet::clientbound::PlayerPositionLook &);
    virtual s32 PacketId(packet::clientbound::KeepAlive &);
    virtual s32 PacketId(packet::clientbound::ChunkData &);
    virtual s32 PacketId(packet::clientbound::SetSlot &);
    virtual s32 PacketId(packet::clientbound::SpawnMob &);
    virtual s32 PacketId(packet::clientbound::DestroyEntities &);
    virtual s32 PacketId(packet::clientbound::EntityVelocity &);

protected:
    ProtocolMap m_InboundMap;
    // Protocol number
    s32 m_VersionNumber;
    s32 m_State;
};

}  // namespace mcidle

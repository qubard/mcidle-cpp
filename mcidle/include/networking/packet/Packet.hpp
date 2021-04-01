#pragma once

#include <networking/ByteBuffer.hpp>
#include <networking/game/GameState.hpp>

namespace mcidle {

class Protocol;

class Packet
{
public:
    Packet();
    Packet(std::shared_ptr<ByteBuffer>);

    Packet &SetProtocol(s32);
    Packet &SetId(s32);
    Packet &SetRawBuffer(std::shared_ptr<ByteBuffer>);
    Packet &SetFieldBuffer(std::shared_ptr<ByteBuffer>);
    Packet &SetGameState(std::shared_ptr<mcidle::game::GameState> &);

    std::shared_ptr<ByteBuffer> RawBuffer();

    // Serialize the packet's fields into `m_Buf`
    virtual Packet &Serialize();
    // Deserialize the packet from a ByteBuffer& into fields
    virtual void Deserialize(ByteBuffer &);

    // Mutate the game state given the packet's information
    virtual void Mutate(mcidle::game::GameState &);

    // Handle a packet using the handler and return a packet
    // as forwarding output
    virtual std::shared_ptr<Packet> Response(Protocol &, s32);

    // Reserve space in `m_FieldBuf`
    Packet &Reserve(std::size_t);

    // Exposes a pointer to the serialized buffer
    // TODO: rename this to outbound/serialized buffer or something
    std::shared_ptr<ByteBuffer> Buffer();
    std::shared_ptr<ByteBuffer> FieldBuffer();

    s32 Id();

    // Write the packet and compress if necessary
    void Write(s32);

protected:
    s32 m_Protocol;
    // The packet's VarInt encoded id
    s32 m_Id;
    // The field or data buffer for serialization
    std::shared_ptr<ByteBuffer> m_FieldBuf;
    // Packet uses game state to serialize/deserialize with context
    std::shared_ptr<mcidle::game::GameState> m_State;

private:
    // The final serialized outbound packet buffer
    std::shared_ptr<ByteBuffer> m_PacketBuf;
    // The raw inbound packet buffer
    std::shared_ptr<ByteBuffer> m_RawRecBuf;
};

}  // namespace mcidle

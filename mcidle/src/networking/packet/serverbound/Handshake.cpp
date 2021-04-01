#include <networking/packet/serverbound/Handshake.hpp>
#include <networking/types/VarInt.hpp>

namespace mcidle {
namespace packet {
    namespace serverbound {

        Packet &Handshake::Serialize()
        {
            *m_FieldBuf << m_ProtocolVersion;
            *m_FieldBuf << m_ServerAddress;
            *m_FieldBuf << m_ServerPort;
            *m_FieldBuf << m_NextState;
            return *this;
        }

        void Handshake::Deserialize(ByteBuffer &buf)
        {
            buf >> m_ProtocolVersion;
            buf >> m_ServerAddress;
            buf >> m_ServerPort;
            buf >> m_NextState;
        }

        s32 Handshake::Protocol() const
        {
            return m_ProtocolVersion.Value();
        }

        std::string Handshake::Address() const
        {
            return m_ServerAddress;
        }

        u16 Handshake::Port() const
        {
            return m_ServerPort;
        }

        s32 Handshake::NextState() const
        {
            return m_NextState.Value();
        }

    }  // namespace serverbound
}  // namespace packet
}  // namespace mcidle

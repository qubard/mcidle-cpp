#include <iostream>
#include <networking/packet/clientbound/EncryptionRequest.hpp>

namespace mcidle {
namespace packet {
    namespace clientbound {

        EncryptionRequest::EncryptionRequest()
        {
        }

        EncryptionRequest::EncryptionRequest(std::string id, std::string pubKey,
                                             std::string token)
            : Packet()
            , m_ServerId(id)
            , m_PubKey(pubKey)
            , m_Token(token)
        {
        }

        Packet &EncryptionRequest::Serialize()
        {
            *m_FieldBuf << m_ServerId;
            *m_FieldBuf << m_PubKey;
            *m_FieldBuf << m_Token;

            return *this;
        }

        void EncryptionRequest::Deserialize(ByteBuffer &buf)
        {
            buf >> m_ServerId;
            buf >> m_PubKey;
            buf >> m_Token;
        }

        std::string EncryptionRequest::PubKey() const
        {
            return m_PubKey;
        }

        std::string EncryptionRequest::Token() const
        {
            return m_Token;
        }

        std::string EncryptionRequest::ServerId() const
        {
            return m_ServerId;
        }

    }  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

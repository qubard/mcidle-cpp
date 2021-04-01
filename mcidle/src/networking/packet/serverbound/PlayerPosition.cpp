#include <networking/packet/serverbound/PlayerPosition.hpp>

namespace mcidle {
namespace packet {
    namespace clientbound {

        PlayerPosition::PlayerPosition()
        {
        }

        PlayerPosition::PlayerPosition(double X, double FeetY, double Z,
                                       bool OnGround)
            : m_X(X)
            , m_FeetY(FeetY)
            , m_Z(Z)
            , m_OnGround(OnGround)
        {
        }

        void PlayerPosition::Mutate(mcidle::game::GameState &state)
        {
            printf("PlayerPosition %.2f %.2f %.2f\n", m_X, m_FeetY, m_Z);
        }

        Packet &PlayerPosition::Serialize()
        {
            *m_FieldBuf << m_X;
            *m_FieldBuf << m_FeetY;
            *m_FieldBuf << m_Z;
            *m_FieldBuf << m_OnGround;
            return *this;
        }

        void PlayerPosition::Deserialize(ByteBuffer &buf)
        {
            buf >> m_X;
            buf >> m_FeetY;
            buf >> m_Z;
            buf >> m_OnGround;
        }

    }  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

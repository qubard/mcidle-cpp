#include <networking/packet/clientbound/SetSlot.hpp>

namespace mcidle {
namespace packet {
namespace clientbound {

SetSlot::SetSlot()
{
}

SetSlot::SetSlot(u8 WindowID, s16 SlotNum, Slot Slot)
    : m_WindowID(WindowID), m_SlotNum(SlotNum), m_Slot(Slot)
{
}

void SetSlot::Mutate(mcidle::game::GameState &state)
{
    std::cout << "Received SetSlot\n";
    std::cout << m_FieldBuf->Hex() << "\n";
}

Packet& SetSlot::Serialize()
{
    *m_FieldBuf << m_WindowID;
    *m_FieldBuf << m_SlotNum;
    *m_FieldBuf << m_Slot;
    return *this;
}

void SetSlot::Deserialize(ByteBuffer &buf)
{
    buf >> m_WindowID;
    buf >> m_SlotNum;
    buf >> m_Slot;
}

}  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

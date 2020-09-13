#include <networking/packet/clientbound/SetSlot.hpp>

namespace mcidle {
namespace packet {
namespace clientbound {

SetSlot::SetSlot()
{
}

SetSlot::SetSlot(u8 WindowID, s16 SlotNum, mcidle::Slot Slot)
    : m_WindowID(WindowID), m_SlotNum(SlotNum), m_Slot(Slot)
{
}

void SetSlot::Mutate(mcidle::game::GameState &state)
{
    if (m_Slot.ItemIDShort == -1)
    {
        state.ClearInventorySlot(m_SlotNum);
    } else 
    {
        state.SetInventorySlot(m_SlotNum, m_Slot);
    }
    std::cout << "Received SetSlot\n";
    std::cout << m_FieldBuf->Hex() << "\n";
}

Packet& SetSlot::Serialize()
{
    *m_FieldBuf << m_WindowID;
    *m_FieldBuf << (s16)m_SlotNum;
    *m_FieldBuf << m_Slot;
    return *this;
}

void SetSlot::Deserialize(ByteBuffer &buf)
{
    printf("Deserializing SetSlot..\n");
    buf >> m_WindowID;
    buf >> m_SlotNum;
    buf >> m_Slot;
    std::cout << "Set slot packet: " << buf.Hex() << "\n";
}

}  // namespace clientbound
}  // namespace packet
}  // namespace mcidle

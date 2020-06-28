#include <networking/PacketHandler.hpp>

namespace mcidle {

PacketHandler::PacketHandler()
{
}

std::shared_ptr<Packet> PacketHandler::Handle(packet::serverbound::KeepAlive &packet)
{
    return nullptr;
}

}  // namespace mcidle

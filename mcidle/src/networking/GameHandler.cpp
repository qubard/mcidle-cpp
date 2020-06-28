#include <networking/GameHandler.hpp>

namespace mcidle {

GameHandler::GameHandler()
{
}

std::shared_ptr<Packet> GameHandler::Handle(packet::serverbound::KeepAlive &packet)
{
    return nullptr;
}

}  // namespace mcidle

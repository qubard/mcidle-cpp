#pragma once

#include <networking/PacketHandler.hpp>

namespace mcidle
{

class GameHandler : public PacketHandler
{
public:
    GameHandler();

    std::shared_ptr<Packet> Handle(packet::serverbound::KeepAlive&) override;
};

}; // ns mcidle

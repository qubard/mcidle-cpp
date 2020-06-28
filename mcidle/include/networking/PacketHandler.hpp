#pragma once

#include <memory>

namespace mcidle
{

// Forward declare packets to resolve
// a cyclical dependency
namespace packet 
{
namespace serverbound
{
class KeepAlive;
}
}

class Packet;

class PacketHandler
{
public:
    PacketHandler();

    virtual std::shared_ptr<Packet> Handle(packet::serverbound::KeepAlive&);
};

} // ns mcidle

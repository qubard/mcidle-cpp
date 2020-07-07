#pragma once

#include <networking/connection/Connection.hpp>

namespace mcidle {

class GameConnection : public Connection
{
public:
    GameConnection(std::unique_ptr<TCPSocket>, 
            std::shared_ptr<mcidle::Protocol>, 
            std::size_t);

    bool Setup() override;
};

} // ns mcidle

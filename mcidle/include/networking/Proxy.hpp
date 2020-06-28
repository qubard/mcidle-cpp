#pragma once

#include <networking/Connection.hpp>
#include <networking/PacketHandler.hpp>
#include <networking/game/GameState.hpp>

#include <mutex>

namespace mcidle {

class Proxy
{
public:
    Proxy(std::shared_ptr<Connection>);
    // A proxy reads from `source` and writes to `sink`
    // after it is setup.
    Proxy(std::shared_ptr<Connection>, std::shared_ptr<Connection>);

    void RegisterHandler(std::shared_ptr<PacketHandler>);
    // Run the proxy and pipe from source to sink
    // and mutate local game state if needed
    void Run();

private:
    std::vector<std::shared_ptr<PacketHandler>> m_Handlers;
    // Mutex over game state
    std::recursive_mutex m_StateLock;
    std::shared_ptr<GameState> m_State;

    std::shared_ptr<Connection> m_Source;
    std::shared_ptr<Connection> m_Sink;
};

}  // namespace mcidle

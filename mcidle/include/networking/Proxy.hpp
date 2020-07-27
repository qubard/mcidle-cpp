#pragma once

#include <mutex>

#include <networking/thread/Pipe.hpp>
#include <networking/connection/Connection.hpp>
#include <networking/game/GameState.hpp>

namespace mcidle {

class Proxy
{
public:
    Proxy(std::shared_ptr<Connection>, std::shared_ptr<mcidle::game::GameState>);
    // A proxy reads from `source` and writes to `sink`
    // after it is setup.
    Proxy(std::shared_ptr<Connection>, std::shared_ptr<thread::Pipe>, std::shared_ptr<mcidle::game::GameState>);

    // Run the proxy and pipe from source to sink
    // and mutate local game state if needed
    void Run();
private:
    // Mutex over game state (re-entrant lock)
    std::recursive_mutex m_StateLock;
    std::shared_ptr<mcidle::game::GameState> m_State;

    std::shared_ptr<thread::Pipe> m_Sink;
    std::shared_ptr<Connection> m_Source;
};

}  // namespace mcidle

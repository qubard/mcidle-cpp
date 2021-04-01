#pragma once

#include <mutex>

#include <networking/connection/Connection.hpp>
#include <networking/game/GameState.hpp>
#include <networking/thread/Pipe.hpp>

namespace mcidle {

class Proxy
{
public:
    Proxy(std::shared_ptr<Connection>, std::shared_ptr<mcidle::game::GameState>);
    // A proxy reads from `source` and writes to `sink`
    // after it is setup.
    Proxy(std::shared_ptr<Connection>, std::shared_ptr<thread::Pipe>, std::shared_ptr<mcidle::game::GameState>);

    void SetSourcePipe(std::shared_ptr<thread::Pipe>);

    // Run the proxy and pipe from source to sink
    // and mutate local game state if needed
    void Run();

private:
    std::shared_ptr<mcidle::game::GameState> m_State;

    std::shared_ptr<thread::Pipe> m_Dest;
    std::shared_ptr<Connection> m_Source;
};

}  // namespace mcidle

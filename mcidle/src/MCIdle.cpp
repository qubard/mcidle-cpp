#include <MCIdle.hpp>
#include <networking/TCPSocket.hpp>
#include <networking/Proxy.hpp>

#include <networking/thread/Pipe.hpp>
#include <boost/thread.hpp>
#include <boost/ref.hpp>

#include <common/Rsa.hpp>

#include <networking/protocol/Protocol_1_12_2.hpp>

namespace mcidle
{

MCIdle::MCIdle(bool online, std::string serverIP, std::string port, std::shared_ptr<Protocol> protocolCB, std::shared_ptr<Protocol> protocolSB, util::Yggdrasil yggdrasil) : m_Online(online), m_ServerIP(serverIP), m_Port(port), m_Protocol_CB(protocolCB), m_Protocol_SB(protocolSB), m_State(std::make_shared<game::GameState>()), m_Yggdrasil(yggdrasil)
{
}

std::shared_ptr<game::GameState> MCIdle::GameState()
{
    return m_State;
}

bool MCIdle::Start()
{
    // Start a new connection as a client to the server
    auto socket = std::make_unique<TCPSocket>(m_ServerIP, m_Port);

    // Connect the socket to the server
    if (!socket->Connect()) 
    {
        printf("Failed to connect to socket\n");
        return false;
    }

    m_ServerConn = std::make_shared<SConnection>(m_ServerIP, m_Port, std::move(socket), m_Protocol_CB, m_State, 8129);
    m_ServerConn->SetOnlineMode(m_Online);

    if (!m_ServerConn->Setup(m_Yggdrasil))
    {
        printf("Failed to setup auth\n");
        return false;
    }

    // max number of packets in the backlog
    // we need this because when the client dcs 
    // and we stop the proxy thread the queue is very large
    u32 maxPipeSize = 30;
    
    // Create a pipe for writing to the client sink
    m_Client = std::make_shared<thread::Pipe>(20, maxPipeSize);
    m_Server = std::make_shared<thread::Pipe>(20, maxPipeSize);

    // Start the pipe in another thread
    boost::thread th(boost::ref(*m_Client));
    boost::thread th2(boost::ref(*m_Server));

    // Start a thread for dealing with ticks
    /*boost::thread([&]() {
        auto TICK_RATE = boost::chrono::milliseconds(50);
        for (;;) 
        {
            //m_State->TickEntities();
            boost::this_thread::sleep_for(TICK_RATE);
        }
    });*/

    printf("Starting listen thread..\n");
    boost::thread([&]() {
		for(;;) {
            auto state = m_State;
			auto conn = std::make_shared<mcidle::TCPSocket>(1337);
            printf("Waiting for client..\n");
			conn->Bind();
            printf("Got client..\n");
			// Client is connected, wrap it in a mc connection object
			auto protocol = std::make_shared<mcidle::Protocol_1_12_2_SB>(340);
            auto mc_conn = std::make_shared<mcidle::Connection>(conn, protocol, state, 4096);
            auto tmp = mc_conn->ReadPacket();
			auto handshake = reinterpret_cast<mcidle::packet::serverbound::Handshake*>(tmp.get());
			protocol->SetState(mcidle::state::LOGIN);
            auto tmp2 = mc_conn->ReadPacket();
			auto loginStart = reinterpret_cast<mcidle::packet::serverbound::LoginStart*>(tmp2.get());

            // Do encryption routines
            if (m_Online)
            {
                // Apparently this is MESSED up.
                mcidle::RSAHelper helper;
                std::string pubKey = helper.Generate1024PubKey();
                // TODO: randomize this
                std::string CToken = "AAAA";
                mc_conn->SendPacket(mcidle::packet::clientbound::EncryptionRequest("", pubKey, CToken));


                // prob getting deallocated as a temporary
                auto pkt = mc_conn->ReadPacket();
                auto encryptionResponse = reinterpret_cast<mcidle::packet::serverbound::EncryptionResponse*>(pkt.get());

                std::string Token, Secret;
                Token.resize(512);
                Secret.resize(512);

                helper.DecryptPrivate(encryptionResponse->Token(), Token);
                helper.DecryptPrivate(encryptionResponse->Secret(), Secret);

                assert(Token == CToken);

                auto aes = std::make_unique<mcidle::AesCtx>();

                bool aesEnabled = aes->InitializeCtx(Secret);
                if (aesEnabled)
                {
                    mc_conn->SetAes(aes);
                    printf("Enabled encryption\n");
                }
                else
                {
                    throw std::runtime_error("Failed to enable encryption in online mode");
                }
            }

            mc_conn->SendPacket(mcidle::packet::clientbound::SetCompression(state->Threshold()));

            printf("Set compression to %d\n", state->Threshold());
            // This is buggy..need to read compression then set the protocol state..
            // or it doesn't matter
            mc_conn->SetCompression(state->Threshold());

            mc_conn->SendPacket(mcidle::packet::clientbound::LoginSuccess("cc78083d-7a7e-40ca-9abb-7edfd2e01383", loginStart->Username()));

            // Make sure join game is identical 
            mcidle::packet::clientbound::JoinGame jg(
                state->PlayerId(),
                state->Gamemode(),
                state->Dimension(),
                state->Difficulty(),
                state->MaxPlayers(),
                state->LevelType(),
                state->DebugInfo()
            );

            // Make sure the state changes here, otherwise the protocol
            // won't decode the right packet types and just crash
            protocol->SetState(mcidle::state::PLAY);

            printf("Sending join game.. dim : %d\n", state->Dimension());
            mc_conn->SendPacket(jg);

            mcidle::packet::clientbound::SpawnPosition sp(state->SpawnX(), state->SpawnY(), state->SpawnZ());

            mcidle::packet::clientbound::PlayerPositionLook pl(state->PlayerX(), state->PlayerY(), state->PlayerZ(), state->PlayerYaw(), state->PlayerPitch(), 0, 19432); // 0 for flags, absolute position

            // Send chunks using their raw buffer
            auto chunks = state->LoadedChunks();

            auto size = mc_conn->SendPacket(pl);
            printf("Sent playerpositionlook size: %lu\n", size);

            for (auto& chunk_ptr : chunks)
            {
                mcidle::packet::clientbound::ChunkData chunkpkt(*chunk_ptr.second);
                mc_conn->SendPacket(chunkpkt);
            }

            size = mc_conn->SendPacket(sp);
            printf("Sent spawn position size: %lu\n", size);

            // Send their inventory
            auto inv = state->PlayerInventory();

            for (auto& slot: inv)
            {
                mcidle::packet::clientbound::SetSlot p(0, slot.first, slot.second);
                printf("About to send set slot for slot num :%d item id short %d\n", slot.first, slot.second.ItemIDShort);
                mc_conn->SendPacket(p);
                printf("Sent set slot for slot num :%d item id short %d\n", slot.first, slot.second.ItemIDShort);
            }

            // Send spawned entities
            auto ents = state->LoadedEntities();

            for (auto e: ents)
            {
                printf("Spawning mob..\n");
                mcidle::packet::clientbound::SpawnMob spawnMob(e.second);
                mc_conn->SendPacket(spawnMob);
                printf("Done Spawning mob..\n");
            }

            // "random keep alive id LOL!"
            s64 keepAliveId = 32919;

            auto keepAlive = mcidle::packet::clientbound::KeepAlive(keepAliveId);

            mc_conn->SendPacket(keepAlive);

            m_Client->Open();
            m_Server->Open();
            // Set the pipe's sink to point at the new client
            m_Client->SetSink(mc_conn, m_Server);

            m_Server->SetSink(m_ServerConn, m_Client);
            // If we hang here then the player joins properly with chunks received
            // The problem is the socket closes before we can do that
            Proxy proxy(mc_conn, m_Server, m_State);

            proxy.Run();

            printf("Reset sinks..\n");

            m_Client->SetSink(nullptr, nullptr);
            m_Server->SetSink(nullptr, nullptr);
        }
        printf("finished\n");
	});

    // Run the proxy which handles packet forwarding
    printf("Starting the proxy..\n");
    Proxy proxy(m_ServerConn, m_Client, m_State);
    proxy.Run();

    printf("Finished proxy run\n");

    return true;
}

} // ns mcidle

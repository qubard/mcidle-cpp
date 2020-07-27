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

MCIdle::MCIdle(bool online, std::string serverIP, std::shared_ptr<Protocol> protocolCB, std::shared_ptr<Protocol> protocolSB, util::Yggdrasil yggdrasil) : m_Online(online), m_ServerIP(serverIP), m_Protocol_CB(protocolCB), m_Protocol_SB(protocolSB), m_State(std::make_shared<game::GameState>()), m_Yggdrasil(yggdrasil)
{
}

std::shared_ptr<game::GameState> MCIdle::GameState()
{
    return m_State;
}

bool MCIdle::Start()
{
    // Start a new connection as a client to the server
    auto socket = std::make_unique<TCPSocket>(m_ServerIP, "25565");

    // Connect the socket to the server
    if (!socket->Connect()) return false;

    m_ServerConn = std::make_shared<SConnection>(std::move(socket), m_Protocol_CB, m_State, 4096);
    m_ServerConn->SetOnlineMode(m_Online);

    if (!m_ServerConn->Setup(m_Yggdrasil))
        return false;

    // Create a pipe for writing to the client sink
    m_Client = std::make_shared<thread::Pipe>(20);
    // Start the pipe in another thread
    boost::thread th(boost::ref(*m_Client));

    boost::thread([&]() {
		for (;;) {
            auto state = m_State;
			auto conn = std::make_shared<mcidle::TCPSocket>(1337);
			conn->Bind();
			// Client is connected, wrap it in a mc connection object
			auto protocol = std::make_shared<mcidle::Protocol_1_12_2_SB>(340);
            auto mc_conn = std::make_shared<mcidle::Connection>(conn, protocol, state, 4096);
			auto handshake = reinterpret_cast<mcidle::packet::serverbound::Handshake*>(mc_conn->ReadPacket().get());
			protocol->SetState(mcidle::state::LOGIN);
			auto loginStart = reinterpret_cast<mcidle::packet::serverbound::LoginStart*>(mc_conn->ReadPacket().get());

            // Do encryption routines
            if (m_Online)
            {
                mcidle::RSAHelper helper;
                std::string pubKey = helper.Generate1024PubKey();
                std::string CToken = "AAAA";
                mc_conn->SendPacket(mcidle::packet::clientbound::EncryptionRequest("", pubKey, CToken));
                auto encryptionresponse = reinterpret_cast<mcidle::packet::serverbound::EncryptionResponse*>(mc_conn->ReadPacket().get());

                std::string Token, Secret;
                Token.resize(512);
                Secret.resize(512);

                helper.DecryptPrivate(encryptionresponse->Token(), Token);
                helper.DecryptPrivate(encryptionresponse->Secret(), Secret);

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
                mcidle::packet::clientbound::ChunkData chunkpkt(std::move(*chunk_ptr.second));
                mc_conn->SendPacket(chunkpkt);
            }

            size = mc_conn->SendPacket(sp);
            printf("Sent spawn position size: %lu\n", size);

            s64 keepAliveId = 32919;

            auto keepAlive = mcidle::packet::clientbound::KeepAlive(keepAliveId);

            mc_conn->SendPacket(keepAlive);

            // Set the sink to point at this client
            m_Client->SetSink(mc_conn);

            // If we hang here then the player joins properly with chunks received
            // The problem is the socket closes before we can do that
            for (;;)
            {
                auto packet = mc_conn->ReadPacket();
                if (packet != nullptr)
                {
                    printf("Received from client: 0x%x\n", packet->Id());
                    std::cout << packet->RawBuffer()->Hex() << "\n";
                }
                else
                {
                    break;
                }
            }
            m_Client->SetSink(nullptr);
        }
        printf("finished\n");
	});

    // Run the proxy which handles packet forwarding
    Proxy proxy(m_ServerConn, m_Client, m_State);
    proxy.Run();

    return true;
}

} // ns mcidle

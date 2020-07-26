#include <networking/connection/SConnection.hpp>

#include <iostream>

namespace mcidle {

SConnection::SConnection(std::unique_ptr<TCPSocket> socket, 
        std::shared_ptr<mcidle::Protocol> protocol,
        std::shared_ptr<mcidle::game::GameState> state,
       std::size_t readSize)
    : Connection(std::move(socket), protocol, state, readSize),
    m_ServerIP("localhost"), m_OnlineMode(false)
{
}

void SConnection::SetServerIP(std::string serverIP)
{
    m_ServerIP = serverIP;
}

void SConnection::SetOnlineMode(bool onlineMode)
{
    m_OnlineMode = onlineMode;
}

bool SConnection::Setup(mcidle::util::Yggdrasil & yg)
{
    SendPacket(mcidle::packet::serverbound::Handshake(340, m_ServerIP, 25565, mcidle::state::LOGIN));

    // Set our next state to LOGIN
    m_Protocol->SetState(mcidle::state::LOGIN);

    SendPacket(mcidle::packet::serverbound::LoginStart("leddit"));

    if (m_OnlineMode)
    {
        auto pkt = ReadPacket();

        if (pkt == nullptr) return false;

        auto response = reinterpret_cast<mcidle::packet::clientbound::EncryptionRequest*>(pkt.get());

        auto aes = std::make_unique<mcidle::AesCtx>();
        auto token = response->Token();
        auto pubKey = response->PubKey();
        auto serverId = response->ServerId();

        if (!aes->Initialize(pubKey, token))
            return false;

        if (!yg.JoinServer(serverId, aes->Secret(), pubKey))
            return false;

        SendPacket(mcidle::packet::serverbound::EncryptionResponse(aes->EncSecret(), aes->EncToken()));
        SetAes(aes);
        printf("Finished encryption.\n");
    }

    // Enable encryption and read a packet
    auto compressionpkt = reinterpret_cast<mcidle::packet::clientbound::SetCompression*>(ReadPacket().get());
    SetCompression(compressionpkt->Threshold());
    m_Protocol->SetState(mcidle::state::PLAY);

    return true;
}

} // ns mcidle

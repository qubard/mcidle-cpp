#include <networking/connection/SConnection.hpp>

#include <iostream>

namespace mcidle {

SConnection::SConnection(std::unique_ptr<TCPSocket> socket, 
        std::shared_ptr<mcidle::Protocol> protocol,
        std::shared_ptr<mcidle::game::GameState> state,
       std::size_t readSize)
    : Connection(std::move(socket), protocol, state, readSize),
    m_ServerName("localhost"), m_OnlineMode(false)
{
}

void SConnection::SetServerName(std::string serverName)
{
    m_ServerName = serverName;
}

void SConnection::SetOnlineMode(bool onlineMode)
{
    m_OnlineMode = onlineMode;
}

bool SConnection::Setup(mcidle::util::Yggdrasil & yg)
{
    mcidle::packet::serverbound::Handshake handshake(340, m_ServerName, 25565, mcidle::state::LOGIN);
    SendPacket(handshake);

    // Set our next state to LOGIN
    m_Protocol->SetState(mcidle::state::LOGIN);

    mcidle::packet::serverbound::LoginStart loginStart("leddit");
    SendPacket(loginStart);

    if (m_OnlineMode)
    {
        auto response = reinterpret_cast<mcidle::packet::clientbound::EncryptionRequest*>(ReadPacket().get());

        auto aes = std::make_unique<mcidle::AesCtx>();
        auto token = response->Token();
        auto pubKey = response->PubKey();

        if (!aes->Initialize(pubKey, token))
            return false;

        if (!yg.JoinServer(response->ServerId(), aes->Secret(), pubKey))
            return false;

        mcidle::packet::serverbound::EncryptionResponse r(aes->EncSecret(), aes->EncToken());
        SendPacket(std::move(r));
        SetAes(aes);
    }

    // Enable encryption and read a packet
    auto compressionpkt = reinterpret_cast<mcidle::packet::clientbound::SetCompression*>(ReadPacket().get());
    SetCompression(compressionpkt->Threshold());
    m_Protocol->SetState(mcidle::state::PLAY);

    return true;
}

} // ns mcidle

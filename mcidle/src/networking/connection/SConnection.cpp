#include <networking/connection/SConnection.hpp>

#include <iostream>

namespace mcidle {

SConnection::SConnection(std::unique_ptr<TCPSocket> socket, 
        std::shared_ptr<mcidle::Protocol> protocol,
       std::size_t readSize)
    : Connection(std::move(socket), protocol, readSize)
{
}

bool SConnection::Setup(mcidle::util::Yggdrasil & yg)
{
    printf("Called SConnection::Setup()\n");

    std::cout << "connected\n"; 

    std::string serverName = "localhost";
    mcidle::packet::serverbound::Handshake handshake(340, serverName, 25565, mcidle::state::LOGIN);
    printf("Sending handshake serverbound\n");
    SendPacket(handshake);
    // handshake 0x00 Handshake(ProtocolVersion=340, ServerAddress='2b2t.org', ServerPort=25565, NextState=2) | 0F 00 D4 02 08 32 62 32 74 2E 6F 72 67 63 DD 02
    std::cout << handshake.Buffer()->Hex() << "\n";
    std::cout << handshake.Buffer()->WriteSize() << "\n";
    // Set our next state to LOGIN
    m_Protocol->SetState(mcidle::state::LOGIN);

    mcidle::packet::serverbound::LoginStart loginStart("leddit");
    SendPacket(loginStart);

    auto encryptionrequest = ReadPacket();
    std::cout << "Encryption request received\n";
    auto er = reinterpret_cast<mcidle::packet::clientbound::EncryptionRequest*>(encryptionrequest.get());

    auto verifytoken = er->Token();
    auto pubkey = er->PubKey();
    auto serverid = er->ServerId();

    for (auto x : verifytoken) std::cout << x << "\n";
    std::cout << verifytoken.size() << " size\n";
    auto aes = std::make_unique<mcidle::AesCtx>();
    
    if (aes->Initialize(pubkey, verifytoken))
    {
        std::cout << "initialized aes\n";
    }
    else {
        std::cout << "failed to init aes\n";
    }

    if (yg.JoinServer(serverid, aes->Secret(), pubkey))
    {
        std::cout << "Joined server\n";
    }
    else 
    {
        std::cout << "Failed to join server\n";
    }

    mcidle::packet::serverbound::EncryptionResponse r(aes->EncSecret(), aes->EncToken());
    SendPacket(std::move(r));
    
    // Enable encryption and read a packet
    auto pkt = SetAes(aes).ReadPacket();
    auto compressionpkt = reinterpret_cast<mcidle::packet::clientbound::SetCompression*>(pkt.get());
    int total = 0;
    std::cout << "Compression: " << compressionpkt->Threshold() << "\n";
    SetCompression(compressionpkt->Threshold());
    m_Protocol->SetState(mcidle::state::PLAY);

    return true;

}

} // ns mcidle

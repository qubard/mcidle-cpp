#include <iostream>
#include <networking/connection/SConnection.hpp>

namespace mcidle
{

SConnection::SConnection(
	std::string serverIP, std::string port, std::unique_ptr<TCPSocket> socket,
	std::shared_ptr<mcidle::Protocol> protocol,
	std::shared_ptr<mcidle::game::GameState> state, std::size_t readSize) :
	Connection(std::move(socket), protocol, state, readSize),
	m_ServerIP(serverIP),
	m_Port(port),
	m_OnlineMode(false)
{
}

void SConnection::SetOnlineMode(bool onlineMode) { m_OnlineMode = onlineMode; }

bool SConnection::Setup(mcidle::util::Yggdrasil & yg)
{
	// TODO: stop hardcoding the port and protocol number
	SendPacket(mcidle::packet::serverbound::Handshake(
		m_Protocol->VersionNumber(), m_ServerIP, std::stoi(m_Port),
		mcidle::state::LOGIN));

	// Set our next state to LOGIN
	m_Protocol->SetState(mcidle::state::LOGIN);

	SendPacket(mcidle::packet::serverbound::LoginStart("leddit"));

	// In online mode encryption is enabled
	if (m_OnlineMode)
	{
		auto pkt = ReadPacket();

		// We got disconnected before getting an EncryptionRequest from the
		// server
		assert(pkt != nullptr);

		auto response =
			reinterpret_cast<mcidle::packet::clientbound::EncryptionRequest *>(
				pkt.get());

		auto aes = std::make_unique<mcidle::AesCtx>();
		auto token = response->Token();
		auto pubKey = response->PubKey();
		auto serverId = response->ServerId();

		if (!aes->Initialize(pubKey, token))
			return false;

		if (!yg.JoinServer(serverId, aes->Secret(), pubKey))
			return false;

		SendPacket(mcidle::packet::serverbound::EncryptionResponse(
			aes->EncSecret(), aes->EncToken()));
		SetAes(aes);
	}

	// Enable encryption and read a packet
	auto compressionpkt =
		reinterpret_cast<mcidle::packet::clientbound::SetCompression *>(
			ReadPacket().get());
	m_State->SetThreshold(compressionpkt->Threshold());
	SetCompression(compressionpkt->Threshold());
	m_Protocol->SetState(mcidle::state::PLAY);

	return true;
}

}  // namespace mcidle

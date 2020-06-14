#include <networking/encryption/AesCtx.hpp>

namespace mcidle {

AesCtx::~AesCtx()
{
	// Need to deallocate here because encryption
	// contexts are structs without customer deleters
	EVP_CIPHER_CTX_free(m_EncryptCtx);
	EVP_CIPHER_CTX_free(m_DecryptCtx);
	
	m_EncryptCtx = nullptr;
	m_DecryptCtx = nullptr;
}

bool AesCtx::Initialize(std::string& publicKey, std::string& verifyToken)
{
	auto pubKeyAddr = &publicKey[0];
	RSA* rsa = d2i_RSA_PUBKEY(NULL, (const u8**)&pubKeyAddr, publicKey.size());
	
	if (rsa == NULL) return false;

	// Generate random shared secret
	std::string sharedSecret;
	for (int i = 0; i < 16; i++)
		sharedSecret += "A";

	s32 rsaSize = RSA_size(rsa);

	// RSA only outputs blocks of size RSA_size(rsa)
	std::string encSecret;
	encSecret.resize(rsaSize);
	std::string encToken;
	encToken.resize(rsaSize);

	// Encrypt the shared secret with public key
	RSA_public_encrypt(sharedSecret.size(), (u8*)&sharedSecret[0], (u8*)&encSecret[0], rsa, RSA_PKCS1_PADDING);
	// Encrypt the verify token with public key
	RSA_public_encrypt(verifyToken.size(), (const u8*)verifyToken.c_str(), (u8*)encToken.c_str(), rsa, RSA_PKCS1_PADDING);
	RSA_free(rsa);

	if (!InitializeCtx(sharedSecret))
		return false;

	m_BlockSize = EVP_CIPHER_block_size(EVP_aes_128_cfb8());

	m_EncSecret = std::move(encSecret);
	m_EncToken = std::move(encToken);
	m_Secret = std::move(sharedSecret);

	return true;
}

bool AesCtx::InitializeCtx(std::string& sharedSecret)
{
	// Initialize AES encryption and decryption
	if (!(m_EncryptCtx = EVP_CIPHER_CTX_new()))
		return false;

	if (!(EVP_EncryptInit_ex(m_EncryptCtx, EVP_aes_128_cfb8(), nullptr, (const u8*)sharedSecret.c_str(), (const u8*)sharedSecret.c_str())))
		return false;

	if (!(m_DecryptCtx = EVP_CIPHER_CTX_new()))
		return false;

	if (!(EVP_DecryptInit_ex(m_DecryptCtx, EVP_aes_128_cfb8(), nullptr, (const u8*)sharedSecret.c_str(), (const u8*)sharedSecret.c_str())))
		return false;

	return true;
}

std::unique_ptr<ByteBuffer> AesCtx::Encrypt(ByteBuffer& buf, s32 size)
{
	auto out = std::make_unique<ByteBuffer>();

	// Ensure we have enough blocks
	out->Resize(size + m_BlockSize);
	EVP_EncryptUpdate(m_EncryptCtx, out->Front(), &size, buf.Front(), size);
	out->Resize(size);
	out->SeekWrite(size);
	return out;
}

std::unique_ptr<ByteBuffer> AesCtx::Decrypt(ByteBuffer& buf, s32 size)
{
	auto out = std::make_unique<ByteBuffer>();

	out->Resize(size + m_BlockSize);
	EVP_DecryptUpdate(m_DecryptCtx, out->Front(), &size, buf.Front(), size);
	out->Resize(size);
	out->SeekWrite(size);
	return out;
}

std::string& AesCtx::EncSecret()
{
	return m_EncSecret;
}

std::string& AesCtx::Secret()
{
	return m_Secret;
}

std::string& AesCtx::EncToken()
{
	return m_EncToken;
}

}
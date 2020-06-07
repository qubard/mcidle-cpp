#pragma once

#include <openssl/aes.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>

#include <networking/ByteBuffer.hpp>

namespace mcidle {

class AesCtx
{
public:
	AesCtx() : m_BlockSize(0), m_EncryptCtx(nullptr), m_DecryptCtx(nullptr) {}
	~AesCtx();

	bool Initialize(std::string&, std::string&);

	std::string& Secret();
	std::string& EncSecret();
	std::string& EncToken();

	std::unique_ptr<ByteBuffer> Encrypt(ByteBuffer&, s32);
	std::unique_ptr<ByteBuffer> Decrypt(ByteBuffer&, s32);
private:
	u32 m_BlockSize;
	EVP_CIPHER_CTX* m_EncryptCtx;
	EVP_CIPHER_CTX* m_DecryptCtx;
	std::string m_EncSecret;
	std::string m_EncToken;
	std::string m_Secret;
};

}

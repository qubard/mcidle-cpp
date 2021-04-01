#pragma once

#include <openssl/bn.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>
#include <iostream>
#include <string>

namespace mcidle {

class RSAHelper
{
public:
    RSAHelper()
    {
        m_Rsa = nullptr;
        m_Pkey = nullptr;
    }

    ~RSAHelper()
    {
        if (m_Pkey != nullptr)
            EVP_PKEY_free(m_Pkey);

        m_Rsa = nullptr;
        m_Pkey = nullptr;
    }

    std::string Generate1024PubKey()
    {
        std::string key;
        m_Pkey = EVP_PKEY_new();

        BIGNUM *exp = BN_new();
        BN_set_word(exp, 65537);

        m_Rsa = RSA_new();
        RSA_generate_key_ex(m_Rsa, 1024, exp, NULL);

        EVP_PKEY_assign_RSA(m_Pkey, m_Rsa);

        key.resize(2048);

        auto ptr = key.data();
        // Generate an ANSI x509 DER format pub key with info (PKCS#1)
        int keyLen = i2d_PUBKEY(m_Pkey, (unsigned char **)&ptr);

        key.resize(keyLen);

        BN_free(exp);

        return key;
    }

    bool DecryptPrivate(std::string cipher, std::string &out)
    {
        if (m_Rsa == nullptr)
            return false;

        int outLen =
            RSA_private_decrypt(cipher.size(), (const u8 *)cipher.c_str(), (u8 *)out.c_str(), m_Rsa, RSA_PKCS1_PADDING);
        out.resize(outLen);
        return true;
    }

private:
    RSA *m_Rsa;
    EVP_PKEY *m_Pkey;
};

}  // namespace mcidle

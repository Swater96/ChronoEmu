//
// Chrono Emu (C) 2016
//
// Credits TomRus88
//


#include "SARC4.h"
#include <openssl/sha.h>

SARC4::SARC4(uint8 len)
{
    EVP_CIPHER_CTX_init(&m_ctx);
    EVP_EncryptInit_ex(&m_ctx, EVP_rc4(), nullptr, nullptr, nullptr);
    EVP_CIPHER_CTX_set_key_length(&m_ctx, len);
}

SARC4::SARC4(uint8 *seed, uint8 len)
{
    EVP_CIPHER_CTX_init(&m_ctx);
    EVP_EncryptInit_ex(&m_ctx, EVP_rc4(), nullptr, nullptr, nullptr);
    EVP_CIPHER_CTX_set_key_length(&m_ctx, len);
    EVP_EncryptInit_ex(&m_ctx, nullptr, nullptr, seed, nullptr);
}

SARC4::~SARC4()
{
    EVP_CIPHER_CTX_cleanup(&m_ctx);
}

void SARC4::Init(uint8 *seed)
{
    EVP_EncryptInit_ex(&m_ctx, nullptr, nullptr, seed, nullptr);
}

void SARC4::UpdateData(int len, uint8 *data)
{
    int outlen = 0;
    EVP_EncryptUpdate(&m_ctx, data, &outlen, data, len);
    EVP_EncryptFinal_ex(&m_ctx, data, &outlen);
}

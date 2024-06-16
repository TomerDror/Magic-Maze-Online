#include "EncryptionUtils.h"
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>

std::string EncryptionUtils::encryptAES(const std::string &plaintext, const std::string &key, const std::string &iv) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    int len;
    int ciphertext_len;
    unsigned char ciphertext[1024];

    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, (unsigned char *)key.c_str(), (unsigned char *)iv.c_str());
    EVP_EncryptUpdate(ctx, ciphertext, &len, (unsigned char *)plaintext.c_str(), plaintext.size());
    ciphertext_len = len;
    EVP_EncryptFinal_ex(ctx, ciphertext + len, &len);
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return std::string((char *)ciphertext, ciphertext_len);
}

std::string EncryptionUtils::decryptAES(const std::string &ciphertext, const std::string &key, const std::string &iv) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    int len;
    int plaintext_len;
    unsigned char plaintext[1024];

    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, (unsigned char *)key.c_str(), (unsigned char *)iv.c_str());
    EVP_DecryptUpdate(ctx, plaintext, &len, (unsigned char *)ciphertext.c_str(), ciphertext.size());
    plaintext_len = len;
    EVP_DecryptFinal_ex(ctx, plaintext + len, &len);
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return std::string((char *)plaintext, plaintext_len);
}

std::string EncryptionUtils::encryptRSA(const std::string &plaintext, const std::string &publicKey) {
    BIO *bio = BIO_new_mem_buf(publicKey.data(), publicKey.size());
    RSA *rsa = PEM_read_bio_RSA_PUBKEY(bio, NULL, NULL, NULL);

    int rsa_len = RSA_size(rsa);
    unsigned char *encrypted = (unsigned char *)malloc(rsa_len);
    RSA_public_encrypt(plaintext.size(), (unsigned char *)plaintext.c_str(), encrypted, rsa, RSA_PKCS1_OAEP_PADDING);

    std::string result((char *)encrypted, rsa_len);

    RSA_free(rsa);
    BIO_free(bio);
    free(encrypted);

    return result;
}

std::string EncryptionUtils::decryptRSA(const std::string &ciphertext, const std::string &privateKey) {
    BIO *bio = BIO_new_mem_buf(privateKey.data(), privateKey.size());
    RSA *rsa = PEM_read_bio_RSAPrivateKey(bio, NULL, NULL, NULL);

    int rsa_len = RSA_size(rsa);
    unsigned char *decrypted = (unsigned char *)malloc(rsa_len);
    RSA_private_decrypt(ciphertext.size(), (unsigned char *)ciphertext.c_str(), decrypted, rsa, RSA_PKCS1_OAEP_PADDING);

    std::string result((char *)decrypted);

    RSA_free(rsa);
    BIO_free(bio);
    free(decrypted);

    return result;
}

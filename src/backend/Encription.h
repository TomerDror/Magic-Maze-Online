#ifndef ENCRYPTIONUTILS_H
#define ENCRYPTIONUTILS_H

#include <string>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

class EncryptionUtils {
public:
    // AES encryption
    static std::string encryptAES(const std::string &plaintext, const std::string &key, const std::string &iv);
    static std::string decryptAES(const std::string &ciphertext, const std::string &key, const std::string &iv);

    // RSA encryption
    static std::string encryptRSA(const std::string &plaintext, const std::string &publicKey);
    static std::string decryptRSA(const std::string &ciphertext, const std::string &privateKey);
};

#endif

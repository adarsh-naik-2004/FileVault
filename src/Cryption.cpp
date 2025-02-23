#include "Cryption.hpp"
#include "Task.hpp"
#include "Env.hpp"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <openssl/evp.h>
#include <vector>
#include <cstring>

void encryptAES(std::fstream &f_stream, const unsigned char *key, const unsigned char *iv) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);

    f_stream.seekg(0, std::ios::end);
    size_t fileSize = f_stream.tellg();
    f_stream.seekg(0, std::ios::beg);

    std::vector<unsigned char> buffer(fileSize);
    f_stream.read(reinterpret_cast<char*>(buffer.data()), fileSize);

    int outLen1 = 0, outLen2 = 0;
    std::vector<unsigned char> encrypted(fileSize + EVP_CIPHER_block_size(EVP_aes_256_cbc()));
    EVP_EncryptUpdate(ctx, encrypted.data(), &outLen1, buffer.data(), fileSize);
    EVP_EncryptFinal_ex(ctx, encrypted.data() + outLen1, &outLen2);
    EVP_CIPHER_CTX_free(ctx);

    f_stream.seekp(0, std::ios::beg);
    f_stream.write(reinterpret_cast<char*>(encrypted.data()), outLen1 + outLen2);
    f_stream.flush();
}

void decryptAES(std::fstream &f_stream, const unsigned char *key, const unsigned char *iv) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);

    f_stream.seekg(0, std::ios::end);
    size_t fileSize = f_stream.tellg();
    f_stream.seekg(0, std::ios::beg);

    std::vector<unsigned char> buffer(fileSize);
    f_stream.read(reinterpret_cast<char*>(buffer.data()), fileSize);

    int outLen1 = 0, outLen2 = 0;
    std::vector<unsigned char> decrypted(fileSize);
    EVP_DecryptUpdate(ctx, decrypted.data(), &outLen1, buffer.data(), fileSize);
    EVP_DecryptFinal_ex(ctx, decrypted.data() + outLen1, &outLen2);
    EVP_CIPHER_CTX_free(ctx);

    f_stream.seekp(0, std::ios::beg);
    f_stream.write(reinterpret_cast<char*>(decrypted.data()), outLen1 + outLen2);
    f_stream.flush();
}

int executeCryption(const std::string &taskData) {
    Task task = Task::fromString(taskData);
    ReadEnv env;
    std::string keyString = env.getEnv();
    unsigned char key[AES_KEYLEN];
    unsigned char iv[AES_BLOCK_SIZE] = {0};

    std::memcpy(key, keyString.c_str(), AES_KEYLEN);

    if (task.action == Action::ENCRYPT) {
        encryptAES(task.f_stream, key, iv);
    } else {
        decryptAES(task.f_stream, key, iv);
    }
    
    task.f_stream.close();

    std::time_t curr_time = std::time(nullptr);
    std::tm* now = std::localtime(&curr_time);

    return 0;
}

#include <filesystem> 
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

bool decryptAES(std::fstream& f_stream, const unsigned char* key, const unsigned char* iv, size_t& decryptedSize) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return false;

    // Initialize decryption
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv)) {
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    // Get encrypted file size
    f_stream.seekg(0, std::ios::end);
    size_t fileSize = f_stream.tellg();
    f_stream.seekg(0, std::ios::beg);

    // Read encrypted data
    std::vector<unsigned char> encryptedData(fileSize);
    f_stream.read(reinterpret_cast<char*>(encryptedData.data()), fileSize);

    // Prepare buffer for decrypted data (account for padding)
    std::vector<unsigned char> decryptedData(fileSize + AES_BLOCK_SIZE);
    int outLen1 = 0, outLen2 = 0;

    // Decrypt data
    if (1 != EVP_DecryptUpdate(ctx, decryptedData.data(), &outLen1, encryptedData.data(), fileSize)) {
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    // Finalize decryption (removes padding)
    if (1 != EVP_DecryptFinal_ex(ctx, decryptedData.data() + outLen1, &outLen2)) {
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    decryptedSize = outLen1 + outLen2; // Actual decrypted data size

    // Write decrypted data to file
    f_stream.seekp(0, std::ios::beg);
    f_stream.write(reinterpret_cast<const char*>(decryptedData.data()), decryptedSize);
    f_stream.flush();

    EVP_CIPHER_CTX_free(ctx);
    return true;
}

int executeCryption(const std::string &taskData) {
    Task task = Task::fromString(taskData);
    ReadEnv env;
    std::string keyString = env.getEnv();

    if (keyString.size() < AES_KEYLEN) {
        std::cerr << "Error: Key must be at least " << AES_KEYLEN << " bytes." << std::endl;
        return -1;
    }

    unsigned char key[AES_KEYLEN];
    unsigned char iv[AES_BLOCK_SIZE] = {0};

    std::memcpy(key, keyString.c_str(), AES_KEYLEN);

    if (task.action == Action::ENCRYPT) {
        encryptAES(task.f_stream, key, iv);
    } else {
        size_t decryptedSize = 0;
        if (!decryptAES(task.f_stream, key, iv, decryptedSize)) {
            std::cerr << "Decryption failed: " << task.filepath << "\n";
            task.f_stream.close();
            return -1;
        }
        
        // Close before truncating
        task.f_stream.close();

        // Truncate file to remove leftover bytes
        std::error_code ec;
        std::filesystem::resize_file(task.filepath, decryptedSize, ec);
        if (ec) {
            std::cerr << "Truncation failed: " << ec.message() << "\n";
            return -1;
        }
    }


    std::time_t curr_time = std::time(nullptr);
    std::tm* now = std::localtime(&curr_time);

    return 0;
}
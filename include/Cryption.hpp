#ifndef CRYPTION_HPP
#define CRYPTION_HPP

#include <string>
#include <fstream>
#include <openssl/aes.h>

#define AES_KEYLEN 32
#define AES_BLOCK_SIZE 16

void encryptAES(std::fstream &f_stream, const unsigned char *key, const unsigned char *iv);
void decryptAES(std::fstream &f_stream, const unsigned char *key, const unsigned char *iv);
int executeCryption(const std::string &taskData);

#endif
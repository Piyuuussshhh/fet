#ifndef ENCRYPT_ALGORITHMS_H
#define ENCRYPT_ALGORITHMS_H

#include <stddef.h>

/**
 * @brief Encrypts a string of content using a simple XOR cipher with a given key.
 * @param plaintext The original plaintext content to encrypt.
 * @param ciphertext_out The buffer where the resulting ciphertext will be stored.
 * @param ciphertext_size The total size of the ciphertext buffer.
 * @param key The password to use for encryption.
 */
void encrypt_XOR(const char *plaintext, char *ciphertext_out, size_t ciphertext_size, const char *key);

#endif // ENCRYPT_ALGORITHMS_H
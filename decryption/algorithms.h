#ifndef DECRYPT_ALGORITHMS_H
#define DECRYPT_ALGORITHMS_H

#include <stddef.h>

/**
 * @brief Decrypts a string of ciphertext using a simple XOR cipher with a given key.
 * Note: For a symmetric XOR cipher, this function is identical to encrypt().
 * @param ciphertext The encrypted content to decrypt.
 * @param plaintext_out The buffer where the resulting plaintext will be stored.
 * @param plaintext_size The total size of the plaintext.
 * @param key The key to use for decryption.
 */
void decrypt_XOR(const char *ciphertext, char *plaintext_out, size_t plaintext_size, const char *key);

#endif // DECRYPT_ALGORITHMS_H
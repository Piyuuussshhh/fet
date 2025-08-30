#include "algorithms.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void encrypt_XOR(const char *plaintext, char *ciphertext_out, size_t ciphertext_size, const char *key) {
    if (key == NULL || *key == '\0') {
        fprintf(stderr, "Error: Encryption key cannot be empty.\n");
        memcpy(ciphertext_out, plaintext, ciphertext_size);
        return;
    }

    // The actual length of the data is one less than the buffer size
    // because the buffer size includes space for the null terminator.
    size_t data_len = ciphertext_size - 1;
    size_t key_len = strlen(key);

    // Copy the raw data first
    memcpy(ciphertext_out, plaintext, data_len);

    // Ensure null termination for safety, though we operate on the known length
    ciphertext_out[data_len] = '\0';

    // Loop using the CORRECT data length, not strlen()
    for (size_t i = 0; i < data_len; i++) {
        ciphertext_out[i] = ciphertext_out[i] ^ key[i % key_len];
    }
}
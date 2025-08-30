#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void decrypt_XOR(const char *ciphertext, char *plaintext_out, size_t plaintext_size, const char *key) {
    if (key == NULL || *key == '\0') {
        fprintf(stderr, "Error: Decryption key cannot be empty.\n");
        memcpy(plaintext_out, ciphertext, plaintext_size);
        return;
    }

    // The actual length of the data is one less than the buffer size
    size_t data_len = plaintext_size - 1;
    size_t key_len = strlen(key);

    // Copy the raw data first
    memcpy(plaintext_out, ciphertext, data_len);

    // Ensure null termination
    plaintext_out[data_len] = '\0';

    // Loop using the CORRECT data length, not strlen()
    for (size_t i = 0; i < data_len; i++) {
        plaintext_out[i] = plaintext_out[i] ^ key[i % key_len];
    }
}
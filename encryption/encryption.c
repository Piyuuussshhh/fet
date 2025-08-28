#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "encryption.h"

/**
 * @brief Encrypts a string of content using a simple XOR cipher with a given key.
 * @param plaintext The original plaintext content to encrypt.
 * @param ciphertext_buffer The buffer where the resulting ciphertext_buffer will be stored.
 * @param buffer_size The total size of the ciphertext_buffer buffer.
 * @param key The password to use for encryption.
 */
void encrypt(const char *plaintext, char *ciphertext_buffer, size_t buffer_size, const char *key) {
    if (key == NULL || *key == '\0') {
        fprintf(stderr, "Error: Encryption key cannot be empty.\n");
        memcpy(ciphertext_buffer, plaintext, buffer_size);
        return;
    }

    // The actual length of the data is one less than the buffer size
    // because the buffer size includes space for the null terminator.
    size_t data_len = buffer_size - 1;
    size_t key_len = strlen(key);

    // Copy the raw data first
    memcpy(ciphertext_buffer, plaintext, data_len);

    // Ensure null termination for safety, though we operate on the known length
    ciphertext_buffer[data_len] = '\0';

    // Loop using the CORRECT data length, not strlen()
    for (size_t i = 0; i < data_len; i++) {
        ciphertext_buffer[i] = ciphertext_buffer[i] ^ key[i % key_len];
    }
}

/**
 * @brief Encrypts file contents using a password, saves to a .etxt file, and deletes the original.
 * @param plaintext The plaintext content read from the original file.
 * @param file_size The size of the file. The reason to pass this is to ensure that every single byte is encrypted
 * and encryption does not stop because the ciphertext_buffer included a `\0` null character by pure chance.
 * @param original_path The full path of the original file (e.g., "C:\\docs\\myfile.txt").
 * @param password The password to be used as the encryption key.
 */
void encrypt_and_delete_txt(const char *plaintext, const size_t file_size, const char *original_path, const char *key) {
    // 1. Create the new filename with the .etxt extension
    char new_path[260];
    strncpy(new_path, original_path, sizeof(new_path) - 1);
    new_path[sizeof(new_path) - 1] = '\0';

    char *dot = strrchr(new_path, '.');
    if (dot != NULL) {
        strcpy(dot, ".etxt");
    } else {
        strcat(new_path, ".etxt");
    }

    // 2. Encrypt the contents
    size_t content_len = file_size;
    char *ciphertext_buffer = (char *)malloc(content_len + 1);
    if (!ciphertext_buffer) {
        fprintf(stderr, "Error: Failed to allocate memory for encryption buffer.\n");
        return;
    }

    printf("Encrypting data...\n");
    encrypt(plaintext, ciphertext_buffer, content_len + 1, key);

    // 3. Write the encrypted contents to the new .etxt file
    FILE *file = fopen(new_path, "wb");
    if (!file) {
        // UPDATED: Use strerror(errno) for a specific error message
        fprintf(stderr, "Error opening file '%s' for writing: %s\n", new_path, strerror(errno));
        free(ciphertext_buffer);
        return;
    }

    fwrite(ciphertext_buffer, 1, content_len, file);
    fclose(file);
    free(ciphertext_buffer);

    printf("Successfully created encrypted file: %s\n", new_path);

    // 4. Delete the original file
    if (remove(original_path) == 0) {
        printf("Successfully deleted original file: %s\n", original_path);
    } else {
        // UPDATED: Use strerror(errno) for a specific error message
        fprintf(stderr, "Error deleting original file '%s': %s\n", original_path, strerror(errno));
    }
}
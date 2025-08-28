#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "decryption.h"

/**
 * @brief Decrypts a string of ciphertext using a simple XOR cipher with a given key.
 * Note: For a symmetric XOR cipher, this function is identical to encrypt().
 * @param ciphertext The encrypted content to decrypt.
 * @param plaintext_buffer The buffer where the resulting plaintext will be stored.
 * @param buffer_size The total size of the plaintext.
 * @param key The key to use for decryption.
 */
void decrypt(const char *ciphertext, char *plaintext_buffer, size_t buffer_size, const char *key) {
    if (key == NULL || *key == '\0') {
        fprintf(stderr, "Error: Decryption key cannot be empty.\n");
        memcpy(plaintext_buffer, ciphertext, buffer_size);
        return;
    }

    // The actual length of the data is one less than the buffer size
    size_t data_len = buffer_size - 1;
    size_t key_len = strlen(key);

    // Copy the raw data first
    memcpy(plaintext_buffer, ciphertext, data_len);

    // Ensure null termination
    plaintext_buffer[data_len] = '\0';

    // Loop using the CORRECT data length, not strlen()
    for (size_t i = 0; i < data_len; i++) {
        plaintext_buffer[i] = plaintext_buffer[i] ^ key[i % key_len];
    }
}

/**
 * @brief Decrypts `.etxt` file contents, saves them to a new `.txt` file, and deletes the original.
 * @param file_contents The encrypted content read from the original file.
 * @param file_size The size of the file. The reason to pass this is to ensure that every single byte is decrypted
 * and decryption does not stop because the ciphertext included a '\0' null character by pure chance.
 * @param original_path The full path of the original .etxt file.
 * @param key The key to be used as the decryption key.
 */
void decrypt_and_create_txt(const char *file_contents, const size_t file_size, const char *original_path, const char *key) {
    // 1. Create the new filename with the .txt extension
    char new_path[260];
    strncpy(new_path, original_path, sizeof(new_path) - 1);
    new_path[sizeof(new_path) - 1] = '\0';

    char *dot = strrchr(new_path, '.');
    if (dot != NULL) {
        strcpy(dot, ".txt");
    } else {
        strcat(new_path, ".txt");
    }

    // 2. Decrypt the contents
    size_t content_len = file_size;
    char *plaintext_buffer = (char *)malloc(content_len + 1);
    if (!plaintext_buffer) {
        fprintf(stderr, "Error: Failed to allocate memory for decryption buffer.\n");
        return;
    }

    printf("Decrypting data...\n");
    decrypt(file_contents, plaintext_buffer, content_len + 1, key);

    // 3. Write the decrypted contents to the new .txt file
    FILE *file = fopen(new_path, "wb");
    if (!file) {
        fprintf(stderr, "Error opening file '%s' for writing: %s\n", new_path, strerror(errno));
        free(plaintext_buffer);
        return;
    }

    fwrite(plaintext_buffer, 1, content_len, file);
    fclose(file);
    free(plaintext_buffer);

    printf("Successfully created decrypted file: %s\n", new_path);

    // 4. Delete the original .etxt file
    if (remove(original_path) == 0) {
        printf("Successfully deleted original file: %s\n", original_path);
    } else {
        fprintf(stderr, "Error deleting original file '%s': %s\n", original_path, strerror(errno));
    }
}
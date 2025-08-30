#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "encryption.h"
#include "../fileio/fileio.h"

void encrypt_and_delete_txt(
    const char *plaintext,
    const size_t file_size,
    const char *original_path,
    const char *key,
    EncryptionFunction encrypt
) {
    // Create the new file's name with the .etxt extension.
    char new_path[260];
    strncpy(new_path, original_path, sizeof(new_path) - 1);
    new_path[sizeof(new_path) - 1] = '\0';

    char *dot = strrchr(new_path, '.');
    if (dot != NULL) {
        strcpy(dot, ".etxt");
    } else {
        strcat(new_path, ".etxt");
    }

    // Encrypt the contents.
    size_t content_len = file_size;
    char *ciphertext_buffer = (char *)malloc(content_len + 1);
    if (!ciphertext_buffer) {
        fprintf(stderr, "Error: Failed to allocate memory for encryption buffer.\n");
        return;
    }

    printf("Encrypting data...\n");
    encrypt(plaintext, ciphertext_buffer, content_len + 1, key);

    // Write the encrypted contents to the new .etxt file.
    if (write_file(new_path, ciphertext_buffer, content_len) != 0) {
        fprintf(stderr, "Error: Failed to write encrypted data to file: %s\n", new_path);
        free(ciphertext_buffer);
        return;
    }

    free(ciphertext_buffer);
    printf("Successfully created encrypted file: %s\n", new_path);

    // Delete the original file
    if (remove(original_path) == 0) {
        printf("Successfully deleted original file: %s\n", original_path);
    } else {
        fprintf(stderr, "Error deleting original file '%s': %s\n", original_path, strerror(errno));
    }
}
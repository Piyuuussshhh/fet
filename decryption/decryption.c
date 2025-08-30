#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "decryption.h"
#include "../fileio/fileio.h"

void decrypt_and_create_txt(
    const char *file_contents,
    const size_t file_size,
    const char *original_path,
    const char *key,
    DecryptionFunction decrypt
) {
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

    // Write the decrypted contents to the new .txt file
    if (write_file(new_path, plaintext_buffer, content_len) != 0) {
        fprintf(stderr, "Error: Failed to write decrypted contents to file '%s'.\n", new_path);
        free(plaintext_buffer);
        return;
    }

    free(plaintext_buffer);
    printf("Successfully created decrypted file: %s\n", new_path);

    // 4. Delete the original .etxt file
    if (remove(original_path) == 0) {
        printf("Successfully deleted original file: %s\n", original_path);
    } else {
        fprintf(stderr, "Error deleting original file '%s': %s\n", original_path, strerror(errno));
    }
}
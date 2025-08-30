#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "encryption/encryption.h"
#include "encryption/algorithms.h"
#include "decryption/decryption.h"
#include "decryption/algorithms.h"
#include "fileio/fileio.h"

int check_arguments(char *argv[]);
int check_extension(const char *filepath, const char *expected_ext);

int main(int argc, char *argv[]) {
    // Make sure number of command-line arguments are correct (there should be 5).
    if (argc < 5 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        printf("Usage:\n\t%s -e path/to/file/to/be/encrypted -k <key>\n", argv[0]);
        printf("\t%s -d path/to/file/to/be/decrypted -k <key>\n", argv[0]);
        return 1;
    }
    // Make sure the arguments are valid.
    if (check_arguments(argv) != 0) {
        return 1;
    }
    // Make sure the file extensions are correct.
    if (strcmp(argv[1], "-e") == 0 && check_extension(argv[2], ".txt") != 0) {
        fprintf(stderr, "Error: The file '%s' does not have a .txt extension.\n", argv[2]);
        return 1;
    }
    if (strcmp(argv[1], "-d") == 0 && check_extension(argv[2], ".etxt") != 0) {
        fprintf(stderr, "Error: The file '%s' does not have a .etxt extension.\n", argv[2]);
        return 1;
    }

    size_t file_size;
    char *file_contents = read_file(argv[2], &file_size);
    if (!file_contents) {
        fprintf(stderr, "Failed to read file: %s\n", argv[2]);
        return 1;
    }
    printf("Read %zu bytes from file '%s'\n", file_size, argv[2]);

    if (strcmp(argv[1], "-e") == 0) {
        encrypt_and_delete_txt(
            file_contents,
            file_size,
            argv[2],
            argv[4],
            encrypt_XOR
        );
    } else if (strcmp(argv[1], "-d") == 0) {
        decrypt_and_create_txt(
            file_contents,
            file_size,
            argv[2],
            argv[4],
            decrypt_XOR
        );
    } else {
        printf("Unknown command: %s\n", argv[1]);
        printf("Usage:\n\t%s -e path/to/file/to/be/encrypted\n", argv[0]);
        printf("\t%s -d path/to/file/to/be/decrypted\n", argv[0]);
        free(file_contents);
        return 1;
    }

    free(file_contents);
    return 0;
}

/**
 * @brief Checks if a filepath ends with a specific extension.
 * @param filepath The full path to the file.
 * @param expected_ext The extension to check for (e.g., ".txt").
 * @return 0 on success (extensions match), -1 on failure.
 */
int check_extension(const char *filepath, const char *expected_ext) {
    // Find the last occurrence of '.' in the filepath
    const char *dot = strrchr(filepath, '.');

    // If there is no dot, or if the dot is the first character
    // then we consider it to have no valid extension.
    if (!dot || dot == filepath) {
        return -1;
    }

    if (strcmp(dot, expected_ext) == 0) {
        return 0; // Success
    }

    return -1;
}

/**
 * @brief Checks if the provided command-line arguments are valid.
 * @param argv The command-line argument array.
 * @return 0 on success (arguments are valid), -1 on failure.
 */
int check_arguments(char *argv[]) {
    if (strcmp(argv[1], "-e") != 0 && strcmp(argv[1], "-d") != 0) {
        fprintf(stderr, "Error: First argument must be -e (encrypt) or -d (decrypt).\n");
        return -1;
    }
    if (strcmp(argv[3], "-k") != 0) {
        fprintf(stderr, "Error: Third argument must be -k to specify the key.\n");
        return -1;
    }
    if (strlen(argv[4]) == 0) {
        fprintf(stderr, "Error: Encryption/Decryption key cannot be empty.\n");
        return -1;
    }
    return 0;
}
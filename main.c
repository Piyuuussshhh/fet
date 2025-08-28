#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "encryption/encryption.h"
#include "decryption/decryption.h"
#include "fileio/fileio.h"

int main(int argc, char *argv[]) {
    if (argc < 5 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        printf("Usage:\n\t%s -e path/to/file/to/be/encrypted -k <key>\n", argv[0]);
        printf("\t%s -d path/to/file/to/be/decrypted -k <key>\n", argv[0]);
        return 1;
    }

    size_t file_size;
    char *file_contents = read_file(argv[2], &file_size);
    printf("Read %zu bytes from file '%s'\n", file_size, argv[2]);
    if (!file_contents) {
        return 1;
    }

    if (strcmp(argv[1], "-e") == 0) {
        encrypt_and_delete_txt(file_contents, file_size, argv[2], argv[4]);
    } else if (strcmp(argv[1], "-d") == 0) {
        decrypt_and_create_txt(file_contents, file_size, argv[2], argv[4]);
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
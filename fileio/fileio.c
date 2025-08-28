#include "fileio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// Reads an entire file into a dynamically allocated buffer.
// Returns a pointer to the buffer, or NULL on error.
// The caller is responsible for freeing the buffer.
char* read_file(const char *path, size_t *file_size) {
    FILE *file = fopen(path, "rb");
    if (!file) {
        fprintf(stderr, "Error opening file '%s': %s\n", path, strerror(errno));
        return NULL;
    }

    // 1. Go to the end of the file to determine its size
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    // 2. CRITICAL: Check for the error value.
    if (size == -1L) {
        perror("Error getting file size with ftell");
        fclose(file);
        return NULL;
    }
    size_t calculated_file_size = (size_t)size;
    fseek(file, 0, SEEK_SET); // Go back to the beginning

    // 2. Allocate enough memory for the whole file + a null terminator
    char *buffer = (char *)malloc(size + 1);
    if (!buffer) {
        fprintf(stderr, "Error: Could not allocate memory for file content: %s\n", strerror(errno));
        fclose(file);
        return NULL;
    }

    // 3. Read the entire file into the buffer
    size_t bytes_read = fread(buffer, 1, calculated_file_size, file);
    if (bytes_read != calculated_file_size) {
        fprintf(stderr, "Error reading file '%s': Incomplete read: %s\n", path, strerror(errno));
        free(buffer);
        fclose(file);
        return NULL;
    }

    // 4. Null-terminate the buffer to make it a valid C string
    buffer[bytes_read] = '\0';
    if (file_size) {
        *file_size = bytes_read;
    }

    fclose(file);
    return buffer;
}


// Writes a string's contents to a file.
// Returns 0 on success, -1 on failure.
int write_file(const char *path, const char *contents) {
    FILE *file = fopen(path, "wb");
    if (!file) {
        fprintf(stderr, "Error opening file '%s' for writing: %s\n", path, strerror(errno));
        return -1;
    }

    size_t len = strlen(contents);
    if (fwrite(contents, 1, len, file) != len) {
        fprintf(stderr, "Error writing to file '%s': Incomplete write: %s\n", path, strerror(errno));
        fclose(file);
        return -1;
    }

    fclose(file);
    return 0;
}
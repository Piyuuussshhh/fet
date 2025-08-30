#define _DEFAULT_SOURCE // For strdup()
#define _POSIX_C_SOURCE 200809L // For using a modern POSIX standard.

#include "fileio.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include <libgen.h>
#include <fcntl.h>



int _safety_checks(const char*, int);

char* read_file(const char *path, size_t *file_size_out) {
    /*
        To make this function safer, I need to:
        1. Check if the path provided is a valid file path.
        2. Check if the file exists and the user executing the program has read permissions.
    */
    // Perform safety checks on the file path.
    // Includes checks for directory traversal and absolute paths.
    if (_safety_checks(path, 0) != 0) {
        fprintf(stderr, "Safety checks failed for path: %s\n", path);
        return NULL;
    }

    // fopen() checks for read permissions implicitly.
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        if (errno == EACCES) {
            fprintf(stderr, "Error: Permission denied when trying to open '%s'.\n", path);
        } else if (errno == ENOENT) {   // ENOENT stands for ERror NO ENTry (No such file or directory)
            fprintf(stderr, "Error: File '%s' not found.\n", path);
        } else {
            // For all other potential errors
            fprintf(stderr, "An error occurred opening '%s': %s\n", path, strerror(errno));
        }
        return NULL;
    }

    // Determine the file's size.
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    if (size == -1L) {
        perror("Error getting file size with ftell");
        fclose(file);
        return NULL;
    }
    size_t calculated_file_size = (size_t)size;
    fseek(file, 0, SEEK_SET);

    // Allocate enough memory for the whole file + a null terminator.
    char *buffer = (char *)malloc(calculated_file_size + 1);
    if (!buffer) {
        fprintf(stderr, "Error: Could not allocate memory for file content: %s\n", strerror(errno));
        fclose(file);
        return NULL;
    }

    // Read the entire file into the buffer.
    size_t bytes_read = fread(buffer, 1, calculated_file_size, file);
    if (bytes_read != calculated_file_size) {
        fprintf(stderr, "Error reading file '%s': Incomplete read: %s\n", path, strerror(errno));
        free(buffer);
        fclose(file);
        return NULL;
    }

    // Null-terminate the buffer to make it a valid C string.
    buffer[bytes_read] = '\0';
    if (file_size_out) {
        *file_size_out = bytes_read;
    }

    fclose(file);
    return buffer;
}

int write_file(const char *path, const char *contents, size_t content_len) {
    if (_safety_checks(path, 1) != 0) {
        fprintf(stderr, "Safety checks failed for path: %s\n", path);
        return -1;
    }

    int fd = open(path, O_WRONLY | O_CREAT | O_EXCL, 0644);
    if (fd == -1) {
        if (errno == EEXIST) {
            fprintf(stderr, "Error: File '%s' already exists. Will not overwrite.\n", path);
        } else if (errno == EACCES) {
            fprintf(stderr, "Error: Permission denied when trying to create '%s'.\n", path);
        } else {
            fprintf(stderr, "An error occurred opening '%s' for writing: %s\n", path, strerror(errno));
        }
        return -1;
    }

    FILE* file = fdopen(fd, "wb");
    if (!file) {
        fprintf(stderr, "Error converting file descriptor to FILE*: %s\n", strerror(errno));
        close(fd);
        return -1;
    }

    if (fwrite(contents, 1, content_len, file) != content_len) {
        fprintf(stderr, "Error writing to file '%s': %s\n", path, strerror(errno));
        fclose(file);
        return -1;
    }

    fclose(file);
    return 0;
}

/**
 * @brief An internal function to perform security checks on a file path.
 *
 * This function verifies that a given path is safe to operate on by ensuring its
 * canonical, absolute path is located within the current working directory. It has
 * two modes: for reading, it checks the file path itself; for writing, it checks
 * the file's parent directory to ensure the location is safe.
 *
 * @param path The file path to validate.
 * @param to_write A flag indicating the operation type: 0 for a read check, and
 * 1 for a write check.
 * @return 0 if the path is deemed safe, or -1 if it is unsafe or an error occurs.
 */
int _safety_checks(const char* path, int to_write) {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("Error getting current working directory");
        return -1;
    }

    char *path_to_check = NULL;
    char *resolved_path = NULL;
    int result = -1;

    if (to_write) {
        // Create a copy to pass to dirname(), which can modify it.
        char *path_copy = strdup(path);
        if (path_copy == NULL) {
            perror("strdup failed");
            return -1;
        }
        // Get the parent directory from the copy.
        path_to_check = dirname(path_copy);
        resolved_path = realpath(path_to_check, NULL);
        free(path_copy); // Free the copy now that we are done with it.
    } else {
        resolved_path = realpath(path, NULL);
    }

    if (resolved_path == NULL) {
        fprintf(stderr, "Error: Invalid path '%s' cannot be resolved.\n", path);
        return -1;
    }

    // Perform the check against the current working directory.
    if (strncmp(resolved_path, cwd, strlen(cwd)) != 0) {
        fprintf(stderr, "Error: Access to '%s' is denied (outside the current directory).\n", resolved_path);
    } else {
        result = 0; // Success
    }

    free(resolved_path); // Free the memory from realpath().
    return result;
}
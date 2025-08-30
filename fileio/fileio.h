#ifndef FILEIO_H
#define FILEIO_H

#include <stddef.h>

/**
 * @brief Reads an entire file into a dynamically allocated buffer.
 *
 * This function performs security checks to ensure the requested path is safe before
 * proceeding. It reads the file in binary mode and adds a null terminator to the
 * end of the buffer, allowing the contents to be treated as a C-string. The caller
 * is responsible for freeing the memory allocated for the returned buffer.
 *
 * @param path The path to the file to be read.
 * @param file_size_out An output parameter; a pointer to a size_t where the file's
 * size in bytes will be stored.
 * @return A pointer to the dynamically allocated buffer containing the file
 * contents, or NULL if any error occurs.
 */
char* read_file(const char *path, size_t *file_size);

/**
 * @brief Safely creates a new file and writes a block of content to it.
 *
 * This function first performs security checks to ensure the file will be created
 * within the current working directory. It uses an atomic open call (O_EXCL) to
 * guarantee that it only creates a new file and will not overwrite an existing one,
 * which prevents race condition vulnerabilities.
 *
 * @param path The path where the new file will be created.
 * @param contents A pointer to the data buffer to be written.
 * @param content_len The number of bytes to write from the contents buffer.
 * @return 0 on success, or -1 on failure.
 */
int write_file(const char *path, const char *contents, size_t content_len);

#endif // FILEIO_H
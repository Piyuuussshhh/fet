#ifndef FILEIO_H
#define FILEIO_H

#include <stddef.h>

// Reads the contents of the file at 'path' into 'buffer'.
// Returns the number of bytes read, or -1 on error.
char* read_file(const char *path, size_t *file_size);

// Writes 'contents' to the file at 'path'.
// Returns 0 on success, or -1 on error.
int write_file(const char *path, const char *contents);

#endif // FILEIO_H
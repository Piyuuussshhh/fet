#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <stddef.h>

void encrypt_and_delete_txt(const char *file_contents, const size_t file_size, const char *original_path, const char *key);
void encrypt(const char *file_contents, char *encrypted_contents, size_t buffer_size, const char *key);

#endif // ENCRYPTION_H
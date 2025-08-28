#ifndef DECRYPTION_H
#define DECRYPTION_H

#include <stddef.h>

void decrypt_and_create_txt(const char *file_contents, const size_t file_size, const char *original_path, const char *key);
void decrypt(const char *file_contents, char *encrypted_contents, size_t buffer_size, const char *key);

#endif // DECRYPTION_H
#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <stddef.h>

typedef void (*EncryptionFunction)(const char*, char*, size_t, const char*);

/**
 * @brief Encrypts file contents using a password, saves to a .etxt file, and deletes the original.
 * @param plaintext The plaintext content read from the original file.
 * @param file_size The size of the file. The reason to pass this is to ensure that every single byte is encrypted
 * and encryption does not stop because the ciphertext_buffer included a `\0` null character by pure chance.
 * @param original_path The full path of the original file (e.g., "C:\\docs\\myfile.txt").
 * @param password The password to be used as the encryption key.
 * @param encrypt The encryption function to use. The function signature should be `void fn(const char *pt, char *ct_out, size_t ct_size, const char *key)`.
 */
void encrypt_and_delete_txt(
    const char *file_contents,
    const size_t file_size,
    const char *original_path,
    const char *key,
    EncryptionFunction encrypt
);

#endif // ENCRYPTION_H
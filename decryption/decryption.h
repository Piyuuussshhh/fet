#ifndef DECRYPTION_H
#define DECRYPTION_H

#include <stddef.h>

typedef void (*DecryptionFunction)(const char*, char*, size_t, const char*);

/**
 * @brief Decrypts `.etxt` file contents, saves them to a new `.txt` file, and deletes the original.
 * @param file_contents The encrypted content read from the original file.
 * @param file_size The size of the file. The reason to pass this is to ensure that every single byte is decrypted
 * and decryption does not stop because the ciphertext included a '\0' null character by pure chance.
 * @param original_path The full path of the original .etxt file.
 * @param key The key to be used as the decryption key.
 */
void decrypt_and_create_txt(
    const char *file_contents,
    const size_t file_size,
    const char *original_path,
    const char *key,
    DecryptionFunction decrypt
);

#endif // DECRYPTION_H
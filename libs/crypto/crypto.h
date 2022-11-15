#include <stdbool.h>
#include <sys/types.h>
#include <stdint.h>

ssize_t xor(const uint8_t *left, size_t left_len, const uint8_t *right, size_t right_len, uint8_t *output);
ssize_t xor_repeated_key(const uint8_t *input, size_t input_len, const uint8_t *key, size_t key_len, uint8_t *output);
ssize_t decipher_xor_single_byte_key(const uint8_t *input, size_t input_len, char *output, uint8_t *output_key, unsigned int *output_score);
ssize_t decipher_aes_128_ecb(const uint8_t *input, size_t input_len, const uint8_t *key, uint8_t *output);
bool is_aes_ecb_encrypted(const uint8_t *input, size_t input_len);

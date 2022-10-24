#include <sys/types.h>
#include <stdint.h>

ssize_t xor(uint8_t *left, size_t left_len, uint8_t *right, size_t right_len, uint8_t *output);
ssize_t xor_hex(char *left, char *right, char *output);
ssize_t xor_repeated_key(uint8_t *input, size_t input_len, uint8_t *key, size_t key_len, uint8_t *output);
ssize_t decipher_single_byte_xor(char *input_hex, char *output, uint8_t *key);
unsigned int english_score(char *input);

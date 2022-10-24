#include <stdint.h>
#include <sys/types.h>

int8_t hex_char_to_byte(char c);
char byte_to_hex_char(uint8_t b);
ssize_t decode_hex(char *input, uint8_t *output);
ssize_t encode_hex(uint8_t *input, size_t input_len, char *output);
ssize_t hex_to_base64(char *input, char *output);
ssize_t xor(uint8_t *left, size_t left_len, uint8_t *right, size_t right_len, uint8_t *output);
ssize_t xor_hex(char *left, char *right, char *output);
ssize_t xor_repeated_key(uint8_t *input, size_t input_len, uint8_t *key, size_t key_len, uint8_t *output);
ssize_t decipher_single_byte_xor(char *input_hex, char *output);
unsigned int english_score(char *input);

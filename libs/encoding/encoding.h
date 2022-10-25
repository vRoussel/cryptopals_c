#include <stdint.h>
#include <sys/types.h>

int8_t hex_char_to_byte(char c);
char byte_to_hex_char(uint8_t b);
ssize_t decode_hex(const char *input, uint8_t *output);
ssize_t encode_hex(const uint8_t *input, size_t input_len, char *output);
ssize_t encode_b64(const uint8_t *input, uint8_t input_len, char *output);
ssize_t hex_to_base64(const char *input, char *output);

#include <stdint.h>
#include <sys/types.h>

int8_t hex_char_to_byte(char c);
char byte_to_hex_char(uint8_t b);
ssize_t decode_hex(char *input, uint8_t *output);
ssize_t encode_hex(uint8_t *input, size_t input_len, char *output);
ssize_t hex_to_base64(char *input, char *output);

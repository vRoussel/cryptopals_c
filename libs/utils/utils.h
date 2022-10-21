#include <stdint.h>
#include <sys/types.h>

int8_t hex_char_to_byte(char c);
char byte_to_hex_char(uint8_t b);
ssize_t hex_to_base64(char *input, char *output);
ssize_t xor(char *left, char *right, char *output);

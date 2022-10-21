#include "utils.h"

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>

//TODO add tests

const char b64_char[64] = {
    'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
    'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
    '0','1','2','3','4','5','6','7','8','9',
    '+','/'
};

int8_t hex_char_to_byte(char c)
{
    char c_up = toupper(c);
    if (isdigit((int)c_up))
        return c_up - '0';
    else if (c_up >= 'A' && c_up <= 'F')
        return c_up - 'A' + 10;
    else
        return -1;
}

int hex_to_base64(char *input, char *output)
char byte_to_hex_char(uint8_t b)
{
    if (b > 15)
        return -1;
    else if (b < 10)
        return '0' + b;
    else
        return 'A' + b - 10;
}
    size_t input_len = strlen(input);
    size_t max_bytes = (input_len + 1) / 2; // 1 byte == 2 hex char; +1 in case input_len is odd
    max_bytes += 2; // in case we need padding
    uint8_t *bytes = malloc(sizeof(uint8_t) * max_bytes);
    size_t bytes_len = 0;

    // If odd number of hex digits, prepend with 0
    if (input_len % 2 == 1) {
        bytes[bytes_len++] = hex_char_to_byte(*input++);
        input_len--;
    }

    assert(input_len % 2 == 0);
    while (input_len >= 2) {
        uint8_t l = hex_char_to_byte(*input++) << 4;
        uint8_t r = hex_char_to_byte(*input++);
        bytes[bytes_len++] = l | r;
        input_len -= 2;
    }

    size_t output_len = 0;
    size_t bytes_i = 0;
    while (bytes_i + 3 <= bytes_len) {
        uint8_t b1 = bytes[bytes_i++];
        uint8_t b2 = bytes[bytes_i++];
        uint8_t b3 = bytes[bytes_i++];
        output[output_len++] = b64_char[(b1 & 0xFC) >> 2];
        output[output_len++] = b64_char[(b1 & 0x03) << 4 | (b2 & 0xF0) >> 4];
        output[output_len++] = b64_char[(b2 & 0x0F) << 2 | (b3 & 0xC0) >> 6];
        output[output_len++] = b64_char[b3 & 0x3F];
    }

    size_t remaining = bytes_len - bytes_i;
    assert(remaining >= 0 && remaining < 3);
    if (remaining == 2) {
        uint8_t b1 = bytes[bytes_i++];
        uint8_t b2 = bytes[bytes_i++];
        output[output_len++] = b64_char[(b1 & 0xFC) >> 2];
        output[output_len++] = b64_char[(b1 & 0x03) << 4 | (b2 & 0xF0) >> 4];
        output[output_len++] = b64_char[(b2 & 0x0F) << 2];
        output[output_len++] = '=';
    } else if (remaining == 1) {
        uint8_t b1 = bytes[bytes_i++];
        output[output_len++] = b64_char[(b1 & 0xFC) >> 2];
        output[output_len++] = b64_char[(b1 & 0x03) << 4];
        output[output_len++] = '=';
        output[output_len++] = '=';
    }
    output[output_len++] = '\0';
    assert(bytes_i == bytes_len);
    assert((output_len - 1) % 4 == 0);

    free(bytes);
    return output_len - 1;
}


#include "encoding.h"

#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

const char b64_char[64] = {
    'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
    'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
    '0','1','2','3','4','5','6','7','8','9',
    '+','/'
};

char byte_to_b64_char(uint8_t b)
{
    if (b > 63)
        return -1;
    return b64_char[b];
}

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

char byte_to_hex_char(uint8_t b)
{
    if (b > 15)
        return -1;
    else if (b < 10)
        return '0' + b;
    else
        return 'A' + b - 10;
}

ssize_t decode_hex(const char *input, uint8_t *output)
{
    int ret = 0;
    size_t input_len = strlen(input);
    size_t output_len = 0;

    // If odd number of hex digits, prepend with 0
    if (input_len % 2 == 1) {
        int8_t b = hex_char_to_byte(*input++);
        if (b < 0) {
            ret = -1;
            goto error;
        }
        output[output_len++] = b;
        input_len--;
    }

    assert(input_len % 2 == 0);
    while (input_len >= 2) {
        int8_t l = hex_char_to_byte(*input++);
        int8_t r = hex_char_to_byte(*input++);
        if (l < 0 || r < 0) {
            ret = -1;
            goto error;
        }
        output[output_len++] = ((uint8_t)l) << 4 | (uint8_t)r;
        input_len -= 2;
    }
    ret = output_len;

error:
    return ret;
}

ssize_t encode_hex(const uint8_t *input, size_t input_len, char *output)
{
    size_t output_len = 0;
    for (size_t i = 0; i < input_len; i++) {
        uint8_t b = input[i];
        uint8_t b1 = (b & 0xF0) >> 4;
        uint8_t b2 = b & 0x0F;
        char c1 = byte_to_hex_char(b1);
        char c2 = byte_to_hex_char(b2);
        if (c1 < 0 || c2 < 0)
            return -1;
        output[output_len++] = c1;
        output[output_len++] = c2;
    }
    output[output_len] = '\0';
    return output_len;
}

ssize_t encode_b64(const uint8_t *input, uint8_t input_len, char *output)
{
    size_t output_len = 0;
    size_t input_i = 0;
    while (input_i + 3 <= input_len) {
        uint8_t b1 = input[input_i++];
        uint8_t b2 = input[input_i++];
        uint8_t b3 = input[input_i++];
        output[output_len++] = byte_to_b64_char((b1 & 0xFC) >> 2);
        output[output_len++] = byte_to_b64_char((b1 & 0x03) << 4 | (b2 & 0xF0) >> 4);
        output[output_len++] = byte_to_b64_char((b2 & 0x0F) << 2 | (b3 & 0xC0) >> 6);
        output[output_len++] = byte_to_b64_char(b3 & 0x3F);
    }

    size_t remaining = input_len - input_i;
    assert(remaining < 3);
    if (remaining == 2) {
        uint8_t b1 = input[input_i++];
        uint8_t b2 = input[input_i++];
        output[output_len++] = byte_to_b64_char((b1 & 0xFC) >> 2);
        output[output_len++] = byte_to_b64_char((b1 & 0x03) << 4 | (b2 & 0xF0) >> 4);
        output[output_len++] = byte_to_b64_char((b2 & 0x0F) << 2);
        output[output_len++] = '=';
    } else if (remaining == 1) {
        uint8_t b1 = input[input_i++];
        output[output_len++] = byte_to_b64_char((b1 & 0xFC) >> 2);
        output[output_len++] = byte_to_b64_char((b1 & 0x03) << 4);
        output[output_len++] = '=';
        output[output_len++] = '=';
    }
    assert(input_i == input_len);
    assert((output_len) % 4 == 0);
    output[output_len] = '\0';
    return output_len;
}

ssize_t hex_to_base64(const char *input, char *output)
{
    int ret = 0;
    size_t input_len = strlen(input);
    size_t max_bytes = (input_len + 1) / 2; // 1 byte == 2 hex char; +1 in case input_len is odd
    max_bytes += 2; // in case we need padding
    uint8_t *bytes = malloc(sizeof(uint8_t) * max_bytes);

    ssize_t tmp = decode_hex(input, bytes);
    if (tmp < 0) {
        ret = -1;
        goto error;
    }
    size_t bytes_len = (size_t)tmp;
    ret = encode_b64(bytes, bytes_len, output);

error:
    free(bytes);
    return ret;
}

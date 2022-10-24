#include "utils.h"

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

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

char byte_to_hex_char(uint8_t b)
{
    if (b > 15)
        return -1;
    else if (b < 10)
        return '0' + b;
    else
        return 'A' + b - 10;
}

ssize_t decode_hex(char *input, uint8_t *output)
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

ssize_t encode_hex(uint8_t *input, size_t input_len, char *output)
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

ssize_t hex_to_base64(char *input, char *output)
{
    int ret = 0;
    size_t output_len = 0;
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
    assert(remaining < 3);
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
    assert(bytes_i == bytes_len);
    assert((output_len) % 4 == 0);
    ret = output_len;

error:
    output[output_len] = '\0';
    free(bytes);
    return ret;
}

ssize_t xor_hex(char *left, char *right, char *output)
{
    size_t left_len = strlen(left);
    size_t right_len = strlen(right);
    size_t left_max_bytes = (left_len + 1) / 2; //+1 in case len is odd
    size_t right_max_bytes = (right_len + 1) / 2; //+1 in case len is odd


    uint8_t left_bytes[left_max_bytes];
    uint8_t right_bytes[right_max_bytes];
    ssize_t left_ret = decode_hex(left, left_bytes);
    ssize_t right_ret = decode_hex(right, right_bytes);

    if (left_ret < 0 || right_ret < 0)
        return -1;

    size_t left_bytes_len = left_ret;
    size_t right_bytes_len = right_ret;
    uint8_t xor_bytes[MAX(left_bytes_len, right_bytes_len)];
    ssize_t xor_ret = xor(left_bytes, left_bytes_len, right_bytes, right_bytes_len, xor_bytes);
    if (xor_ret < 0)
        return -1;
    size_t xor_bytes_len = xor_ret;
    return encode_hex(xor_bytes, xor_bytes_len, output);
}

ssize_t xor(uint8_t *left, size_t left_len, uint8_t *right, size_t right_len, uint8_t *output)
{
    size_t left_i = 0;
    size_t right_i = 0;
    size_t output_len = 0;

    // Consume the longest byte array until both have same length
    size_t remaining_bytes_left = left_len - left_i;
    size_t remaining_bytes_right = right_len - right_i;
    while (remaining_bytes_left < remaining_bytes_right) {
        output[output_len++] = right[right_i++];
        remaining_bytes_right--;
    }
    while (remaining_bytes_right < remaining_bytes_left) {
        output[output_len++] = right[left_i++];
        remaining_bytes_left--;
    }
    assert(remaining_bytes_left == remaining_bytes_right);

    while(left_i < left_len) {
        output[output_len++] = left[left_i++] ^ right[right_i++];
    }
    assert(left_i == left_len);
    assert(right_i == right_len);
    return output_len;
}

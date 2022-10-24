#include "utils.h"

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <limits.h>

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define SWAP(x, y, T) do { T SWAP = x; x = y; y = SWAP; } while (0)

double english_letters_freq[] = {
    8.4966,  //a
    2.0720,  //b
    4.5388,  //c
    3.3844,  //d
    11.1607, //e
    1.8121,  //f
    2.4705,  //g
    3.0034,  //h
    7.5448,  //i
    0.1965,  //j
    1.1016,  //k
    5.4893,  //l
    3.0129,  //m
    6.6544,  //n
    7.1635,  //o
    3.1671,  //p
    0.1962,  //q
    7.5809,  //r
    5.7351,  //s
    6.9509,  //t
    3.6308,  //u
    1.0074,  //v
    1.2899,  //w
    0.2902,  //x
    1.7779,  //y
    0.2722,  //z
};

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

ssize_t xor_repeated_key(uint8_t *input, size_t input_len, uint8_t *key, size_t key_len, uint8_t *output)
{
    size_t input_i = 0;
    size_t key_i = 0;
    size_t output_len = 0;

    while (input_i < input_len) {
        output[output_len++] = input[input_i++] ^ key[key_i++];
        key_i %= key_len;
    }
    return output_len;
}

ssize_t decipher_single_byte_xor(char *input_hex, char *output)
{
    size_t len = strlen(input_hex);
    size_t max_bytes = (len + 1) / 2; //+1 in case len is odd

    uint8_t bytes[max_bytes];
    ssize_t bytes_ret = decode_hex(input_hex, bytes);
    if (bytes_ret < 1)
        return -1;

    size_t bytes_len = bytes_ret;

    char buf1[len];
    char buf2[len];
    char *storage = buf1;
    char *best_match = buf2;
    unsigned int best_score = 100;
    ssize_t ret = -1;
    for (uint16_t i = 0; i <= UINT8_MAX; i++) {
        uint8_t key = i;
        ssize_t xor_ret = xor_repeated_key(bytes, bytes_len, &key, 1, (uint8_t*)storage);
        if (xor_ret >= 0) {
            unsigned int score = english_score(storage);
            if (score < best_score) {
                best_score = score;
                ret = xor_ret;
                SWAP(best_match, storage, char*);
            }
        }
    }
    if (ret >= 0) {
        memcpy(output, best_match, ret);
        output[ret] = '\0';
    }
    return ret;
}

unsigned int english_score(char *input)
{
    unsigned int letters_count[26] = {0};
    unsigned int total_count = 0;
    while (*input != '\0') {
        char c = tolower(*input);
        if (c >= 'a' && c <= 'z') {
            letters_count[c - 'a']++;
        }
        total_count++;
        input++;
    }

    if (total_count == 0)
        return 100;

    double score = 0;
    for (int i = 0; i < 26; i++) {
        double input_freq = letters_count[i] / (double)total_count * 100;
        double english_freq = english_letters_freq[i];
        score += fabs(english_freq - input_freq);
    }
    return (unsigned int)score;
}

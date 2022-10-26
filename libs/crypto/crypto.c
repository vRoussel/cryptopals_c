#include "crypto.h"

#include <limits.h>
#include <string.h>
#include <assert.h>

#include "languages.h"
#include "encoding/encoding.h"
#include "utils/utils.h"

ssize_t xor(const uint8_t *left, size_t left_len, const uint8_t *right, size_t right_len, uint8_t *output)
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

ssize_t xor_hex(const char *left, const char *right, char *output)
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

ssize_t xor_repeated_key(const uint8_t *input, size_t input_len, const uint8_t *key, size_t key_len, uint8_t *output)
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

ssize_t decipher_xor_single_byte_key(const char *input_hex, char *output, uint8_t *output_key, unsigned int *output_score)
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
    *output_score = UINT_MAX;
    ssize_t ret = -1;
    for (uint16_t i = 0; i <= UINT8_MAX; i++) {
        uint8_t key = i;
        ssize_t xor_ret = xor_repeated_key(bytes, bytes_len, &key, 1, (uint8_t*)storage);
        if (xor_ret >= 0) {
            unsigned int score = english_score(storage);
            if (score < *output_score) {
                *output_score = score;
                *output_key = key;
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

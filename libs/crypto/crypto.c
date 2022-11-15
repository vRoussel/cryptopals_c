#include "crypto.h"

#include <limits.h>
#include <string.h>
#include <assert.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#include "languages.h"
#include "encoding/encoding.h"
#include "utils/utils.h"
#include "utils/hashmap.h"

#define ECB_ENCODING_DETECTION_DUPS_TRESHOLD 2

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

ssize_t decipher_xor_single_byte_key(const uint8_t *input, size_t input_len, char *output, uint8_t *output_key, unsigned int *output_score)
{
    char buf1[input_len];
    char buf2[input_len];
    char *storage = buf1;
    char *best_match = buf2;
    unsigned int best_score = UINT_MAX;
    char best_key;
    ssize_t ret = -1;
    for (uint16_t i = 0; i <= UINT8_MAX; i++) {
        uint8_t key = i;
        ssize_t xor_ret = xor_repeated_key(input, input_len, &key, 1, (uint8_t*)storage);
        if (xor_ret >= 0) {
            unsigned int score = english_score(storage);
            if (score < best_score) {
                best_score = score;
                best_key = key;
                ret = xor_ret;
                SWAP(best_match, storage, char*);
            }
        }
    }
    if (ret >= 0) {
        if (output) {
            memcpy(output, best_match, ret);
            output[ret] = '\0';
        }
        if (output_score) {
            *output_score = best_score;
        }
        if (output_key) {
            *output_key = best_key;
        }
    }
    return ret;
}

static void handleOpenSSLErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}

ssize_t decipher_aes_128_ecb(const uint8_t *input, size_t input_len, const uint8_t *key, uint8_t *output)
{
    int len;
    int output_len;
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
        handleOpenSSLErrors();

    if (EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL) != 1)
        handleOpenSSLErrors();

    if (EVP_DecryptUpdate(ctx, output, &len, input, input_len) != 1)
        handleOpenSSLErrors();
    output_len = len;

    if (EVP_DecryptFinal_ex(ctx, output + len, &len) != 1)
        handleOpenSSLErrors();
    output_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return (ssize_t)output_len;
}

bool is_aes_ecb_encrypted(const uint8_t *input, size_t input_len)
{
    if (input_len % 16 != 0)
        return false;

    bool ret = false;
    unsigned int dups = 0;
    hashmap_s hm;
    hashmap_init(&hm);
    bool already_seen = false;
    char chunk[17];
    for (size_t i = 0; i < input_len; i += 16) {
        memcpy(chunk, &input[i], 16);
        chunk[16] = '\0';
        hashmap_get(&hm, chunk, &already_seen);
        if (already_seen) {
            if(++dups >= ECB_ENCODING_DETECTION_DUPS_TRESHOLD) {
                ret = true;
                break;
            }
        } else {
            hashmap_set(&hm, chunk, NULL, 0);
        }
    }
    hashmap_finalize(&hm);
    return ret;
}

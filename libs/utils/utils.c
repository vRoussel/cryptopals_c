#include "utils.h"

#include <stdint.h>
#include <string.h>
#include <limits.h>

#include "crypto/crypto.h"

unsigned int hamming_distance_str(const char* s1, const char* s2)
{
    size_t s1_len = strlen(s1);
    size_t s2_len = strlen(s2);

    if (s1_len != s2_len)
        return -1;

    uint8_t xor_bytes[s1_len];

    ssize_t xor_ret = xor((uint8_t*)s1, s1_len, (uint8_t*)s2, s2_len, xor_bytes);
    if (xor_ret < 0)
        return -1;

    unsigned int diff_count = 0;
    size_t xor_bytes_len = xor_ret;
    for (size_t i = 0; i < xor_bytes_len; i++) {
        uint8_t b = xor_bytes[i];
        for (size_t j = 0; j < CHAR_BIT; j++) {
            diff_count += (b >> j) & 0x1;
        }
    }

    return diff_count;
}

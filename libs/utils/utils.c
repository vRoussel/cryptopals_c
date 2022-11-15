#include "utils.h"

#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

#include "crypto/crypto.h"

unsigned int hamming_distance(const uint8_t *left, const uint8_t *right, size_t size)
{
    uint8_t xor_bytes[size];

    ssize_t xor_ret = xor(left, size, right, size, xor_bytes);
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

unsigned int hamming_distance_str(const char* s1, const char* s2)
{
    size_t s1_len = strlen(s1);
    size_t s2_len = strlen(s2);

    if (s1_len != s2_len)
        return -1;


    return hamming_distance((uint8_t*)s1, (uint8_t*)s2, s1_len);
}

int get_next_line(char *out, size_t limit, unsigned int *line_num)
{
    bool found = false;
    while (!found && fgets(out, limit, stdin) != NULL) {
        if (line_num)
            (*line_num)++;
        size_t len = strlen(out);
        assert(len > 0);
        if (out[len - 1] != '\n') {
            printf("Skipping line %d because it is too long or incomplete (\\n missing?)\n", line_num ? *line_num : 0);
            skip_until_eol();
            continue;
        }
        out[--len] = '\0';
        found = true;
    }
    return found ? 0 : EOF;
}

void skip_until_eol()
{
    for (int c = getchar(); c != '\n' && c != EOF; c = getchar());
}

void *memdup(const void* in, size_t size)
{
    char *dup = malloc(size);

    if (dup != NULL)
        memcpy(dup, in, size);

    return dup;
}

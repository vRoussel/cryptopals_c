#pragma once

#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>

typedef struct {
    char input[BUF_SIZE];
    char output[BUF_SIZE];
    unsigned int score;
    uint8_t key;
} deciphered_s;


void deciphered_init(deciphered_s *d)
{
    d->score = UINT_MAX;
    d->key = 0;
}

void deciphered_finalize(deciphered_s *d)
{
    d->score = UINT_MAX;
    d->key = 0;
}

void deciphered_print(const deciphered_s *d)
{
    printf("%s [key=%hhu] [score=%u] -> %s\n", d->input, d->key, d->score, d->output);
}

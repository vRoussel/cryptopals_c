#pragma once

#include <stdint.h>
#include <stdio.h>

typedef struct {
    char *input;
    char *output;
    unsigned int score;
    uint8_t key;
} deciphered_s;


void deciphered_print(const deciphered_s *d)
{
        printf("%s [key=%hhu] [score=%u] -> %s\n", d->input, d->key, d->score, d->output);
}

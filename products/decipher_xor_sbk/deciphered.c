#include "deciphered.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

void deciphered_init(deciphered_s *d, size_t buf_size)
{
    d->input = malloc(sizeof(char) * buf_size);
    d->output = malloc(sizeof(char) * buf_size);
    d->score = UINT_MAX;
    d->key = 0;
}

void deciphered_finalize(deciphered_s *d)
{
    free(d->input);
    free(d->output);
}

void deciphered_print(const deciphered_s *d)
{
        printf("%s [key=%hhu] [score=%u] -> %s\n", d->input, d->key, d->score, d->output);
}

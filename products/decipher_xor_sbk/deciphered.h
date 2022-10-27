#pragma once

#include <stddef.h>
#include <stdint.h>


typedef struct {
    char *input;
    char *output;
    unsigned int score;
    uint8_t key;
} deciphered_s;


void deciphered_init(deciphered_s *d, size_t buf_size);
void deciphered_finalize(deciphered_s *d);
void deciphered_print(const deciphered_s *d);

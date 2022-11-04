#include <stdio.h>
#include <float.h>
#include <stdlib.h>
#include <string.h>

#include "utils/utils.h"
#include "crypto/crypto.h"
#include "encoding/encoding.h"

#define BUF_SIZE 4096
#define ENGLISH_SCORE_LIMIT 80
#define MIN_KEY_SIZE 1
#define MAX_KEY_SIZE 40

#include "block.h"

unsigned int get_key_size(uint8_t *input);
void print_usage();
void parse_args(int argc, char *argv[]);

//TODO add --base64 and --hex flags

int main(int argc, char *argv[])
{
    parse_args(argc, argv);

    unsigned int line_num = 0;
    char input[BUF_SIZE];
    uint8_t bytes[BUF_SIZE];

    block_s blocks[MAX_KEY_SIZE];
    for (unsigned int i = 0; i < MAX_KEY_SIZE; i++) {
        block_init(&blocks[i]);
    }

    while (get_next_line(input, BUF_SIZE, &line_num) == 0) {
        ssize_t decode_ret = decode_b64(input, bytes);
        if (decode_ret < 0)
            continue;
        size_t bytes_len = decode_ret;
        unsigned int key_size = get_key_size(bytes);
        printf("Key size is probably %u\n", key_size);

        for (size_t j = 0; j < bytes_len; j++) {
            unsigned int idx = j % key_size;
            block_append(&blocks[idx], bytes[j]);
        }

        uint8_t key[MAX_KEY_SIZE];
        for (unsigned int k = 0; k < key_size; k++) {
            block_s *b = &blocks[k];
            ssize_t ret = decipher_xor_single_byte_key(b->buffer, b->len, NULL, &key[k], NULL);
            if (ret < 0) {
                printf("Error while trying to decipher block %u/%u", k+1, key_size);
                continue;
            }
        }
        printf("Key is %s\n", key);
        char output[BUF_SIZE];
        ssize_t ret = xor_repeated_key(bytes, bytes_len, key, key_size, (uint8_t*)output);
        if (ret < 0) {
            printf("Error while trying to decipher input with key %s", key);
            continue;
        }
        output[ret] = '\0';
        printf("Deciphered text is:\n%s\n", output);
    }

    for (unsigned int i = 0; i < MAX_KEY_SIZE; i++) {
        block_finalize(&blocks[i]);
    }
}

unsigned int get_key_size(uint8_t *input)
{
    unsigned int best_key_size;
    double best_hamming_distance = DBL_MAX;
    for (unsigned int key_size = MIN_KEY_SIZE; key_size <= MAX_KEY_SIZE; key_size++) {
        uint8_t *chunk1 = input;
        uint8_t *chunk2 = input + key_size * 1;
        uint8_t *chunk3 = input + key_size * 2;
        uint8_t *chunk4 = input + key_size * 3;

        double hd12_norm = hamming_distance(chunk1, chunk2, key_size) / (double)key_size;
        double hd13_norm = hamming_distance(chunk1, chunk3, key_size) / (double)key_size;
        double hd14_norm = hamming_distance(chunk1, chunk4, key_size) / (double)key_size;
        double hd23_norm = hamming_distance(chunk2, chunk3, key_size) / (double)key_size;
        double hd24_norm = hamming_distance(chunk2, chunk4, key_size) / (double)key_size;
        double hd34_norm = hamming_distance(chunk3, chunk4, key_size) / (double)key_size;
        double hd_avg = (hd12_norm + hd13_norm + hd14_norm + hd23_norm + hd24_norm + hd34_norm) / 6.0;
        if (hd_avg < best_hamming_distance) {
            best_hamming_distance = hd_avg;
            best_key_size = key_size;
        }
    }
    return best_key_size;
}

void print_usage()
{
    printf("decipher_xor_rk (repeating key)\n"
           "Take ciphered b64 strings from stdin and try to find the best repeating key to decipher them (by xoring the two).\n"
           "One b64 string per line. Lines that exceed 4096 chars (newline include) will be skipped.\n"
           "The best combination (key and output produced) will be printed.\n"
           "\n"
           "Usage: ./decipher_xor_rk\n"
    );
}


void parse_args(int argc, char *argv[])
{
    if (argc > 2) {
        print_usage();
        exit(1);
    } else if (argc == 2) {
        if (strcmp(argv[1], "--help") == 0) {
            print_usage();
            exit(0);
        } else {
            print_usage();
            exit(1);
        }
    }
}

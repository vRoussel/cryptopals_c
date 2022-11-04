#include "crypto/crypto.h"
#include "encoding/encoding.h"
#include "utils/utils.h"

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define ENGLISH_SCORE_LIMIT 80
#define BUF_SIZE 4096

#include "deciphered.h"


void print_usage();
void parse_args(int argc, char *argv[]);

static int verbose = 0;

//TODO add --base64 and --hex flags

int main(int argc, char *argv[])
{
    parse_args(argc, argv);

    unsigned int line_num = 0;
    deciphered_s candidate;
    deciphered_init(&candidate);
    while (get_next_line(candidate.input, BUF_SIZE, &line_num) == 0) {
        uint8_t bytes[BUF_SIZE];
        ssize_t decode_ret = decode_hex(candidate.input, bytes);
        if (decode_ret < 1) {
            printf("Unable to decode input on line %d\n", line_num);
            continue;
        }
        size_t bytes_len = decode_ret;

        if (decipher_xor_single_byte_key(bytes, bytes_len, candidate.output, &candidate.key, &candidate.score) < 0) {
            printf("Error while trying to decipher line %d\n", line_num);
            continue;
        }

        if (verbose || candidate.score < ENGLISH_SCORE_LIMIT) {
            deciphered_print(&candidate);
        }
    }
    deciphered_finalize(&candidate);
}

void print_usage()
{
    printf("decipher_xor_sbk (single byte key)\n"
           "Take ciphered hex strings from stdin and try to find the best single byte key to decipher them (by xoring the two).\n"
           "One hex string per line. Lines that exceed 4096 chars (newline include) will be skipped.\n"
           "If one of the solution looks like english (based on a probably-too-simple homemade algorithm), it will be printed.\n"
           "\n"
           "Usage: ./decipher_xor_sbk [--verbose]\n"
           "Flags:\n"
           " --verbose: print the best solution for each line, regardless of its \"english score\"\n"

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
        } else if (strcmp(argv[1], "--verbose") == 0) {
            verbose = 1;
        } else {
            print_usage();
            exit(1);
        }
    }
}

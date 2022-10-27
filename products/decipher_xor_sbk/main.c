#include "crypto/crypto.h"
#include "utils/utils.h"
#include "deciphered.h"

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 4096
#define ENGLISH_SCORE_LIMIT 80

void print_usage();
void skip_until_eol();
void parse_args(int argc, char *argv[]);
int get_next_line(char *out, size_t limit);

static unsigned int line_num = 0;
static int verbose = 0;

int main(int argc, char *argv[])
{
    parse_args(argc, argv);

    deciphered_s candidate;
    deciphered_init(&candidate, BUF_SIZE);
    while (get_next_line(candidate.input, BUF_SIZE) == 0) {
        if (decipher_xor_single_byte_key(candidate.input, candidate.output, &candidate.key, &candidate.score) < 0) {
            printf("Error while trying to decipher line %d\n", line_num);
            continue;
        }

        if (verbose || candidate.score < ENGLISH_SCORE_LIMIT) {
            deciphered_print(&candidate);
        }
    }
    deciphered_finalize(&candidate);
}

void skip_until_eol()
{
    for (int c = getchar(); c != '\n' && c != EOF; c = getchar());
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

int get_next_line(char *out, size_t limit)
{
    bool found = false;
    while (!found && fgets(out, limit, stdin) != NULL) {
        line_num++;
        size_t len = strlen(out);
        assert(len > 0);
        if (out[len - 1] != '\n') {
            printf("Skipping line %d because it is too long or incomplete (\\n missing?)\n", line_num);
            skip_until_eol();
            continue;
        }
        out[--len] = '\0';
        found = true;
    }
    return found ? 0 : EOF;
}

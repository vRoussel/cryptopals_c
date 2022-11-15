#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/utils.h"
#include "crypto/crypto.h"
#include "encoding/encoding.h"

#define BUF_SIZE 4096

void print_usage();
void parse_args(int argc, char *argv[]);

//TODO add --base64 and --hex flags

int main(int argc, char *argv[])
{
    parse_args(argc, argv);

    unsigned int line_num = 0;
    char input[BUF_SIZE];
    uint8_t bytes[BUF_SIZE];

    while (get_next_line(input, BUF_SIZE, &line_num) == 0) {
        ssize_t decode_ret = decode_hex(input, bytes);
        if (decode_ret < 0)
            continue;
        size_t bytes_len = decode_ret;
        if (is_aes_ecb_encrypted(bytes, bytes_len)) {
            printf("%s\n", input);
        }
    }
}

void print_usage()
{
    printf("detect_aes_ecb\n"
           "Take ciphered hex encoded strings from stdin.\n"
           "One per line. Lines that exceed 4096 chars (newline include) will be skipped.\n"
           "Lines that look like aes ecb encoded will be printed back to stdout.\n"
           "\n"
           "Usage: ./detect_aes_ecb\n"
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


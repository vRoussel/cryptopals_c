#include "languages.h"

#include "ctype.h"
#include "math.h"

const double english_letters_freq[] = {
    8.4966,  //a
    2.0720,  //b
    4.5388,  //c
    3.3844,  //d
    11.1607, //e
    1.8121,  //f
    2.4705,  //g
    3.0034,  //h
    7.5448,  //i
    0.1965,  //j
    1.1016,  //k
    5.4893,  //l
    3.0129,  //m
    6.6544,  //n
    7.1635,  //o
    3.1671,  //p
    0.1962,  //q
    7.5809,  //r
    5.7351,  //s
    6.9509,  //t
    3.6308,  //u
    1.0074,  //v
    1.2899,  //w
    0.2902,  //x
    1.7779,  //y
    0.2722,  //z
};

const double english_space_freq = 20.0;

unsigned int english_score(const char *input)
{
    unsigned int letters_count[26] = {0};
    unsigned int total_count = 0;
    unsigned int non_printable_count = 0;
    unsigned int space_count = 0;
    while (*input != '\0') {
        char c = tolower(*input);
        if (c >= 'a' && c <= 'z') {
            letters_count[c - 'a']++;
        } else if (c == ' ') {
            space_count++;
        } else if (!isprint(c)) {
            non_printable_count++;
        }
        total_count++;
        input++;
    }

    if (total_count == 0)
        return 100;

    double score = 0;
    for (int i = 0; i < 26; i++) {
        double input_freq = letters_count[i] / (double)total_count * 100;
        double english_freq = english_letters_freq[i];
        score += fabs(english_freq - input_freq);
    }
    double space_freq = space_count / (double)total_count * 100;
    score += fabs(space_freq - english_space_freq);
    score += (non_printable_count / (double)total_count * 100);
    return (unsigned int)score;
}

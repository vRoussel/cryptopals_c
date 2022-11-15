#include <stddef.h>
#include <stdint.h>

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define SWAP(x, y, T) do { T SWAP = x; x = y; y = SWAP; } while (0)

unsigned int hamming_distance(const uint8_t *left, const uint8_t *right, size_t size);
unsigned int hamming_distance_str(const char* s1, const char* s2);
int get_next_line(char *out, size_t limit, unsigned int *line_num);
void skip_until_eol();
void *memdup(const void* in, size_t size);

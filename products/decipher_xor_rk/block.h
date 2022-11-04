#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint8_t buffer[BUF_SIZE];
    size_t len;
} block_s;

void block_init(block_s *block)
{
    block->len = 0;
}

void block_finalize(block_s *block)
{
    block->len = 0;
}

int block_append(block_s *block, char c)
{
    if (block->len >= BUF_SIZE)
        return -1;

    block->buffer[block->len++] = c;
    return 0;
}

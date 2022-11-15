#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define HASHMAP_DEFAULT_BUCKET_COUNT 8
#define HASHMAP_MAX_KEY_BUCKET_RATIO 0.75

typedef struct node_s {
    struct node_s *next;
    char *key;
    void *value;
} node_s;

typedef struct {
    node_s *buckets;
    size_t n_buckets;
    size_t n_keys;
} hashmap_s;

void hashmap_init(hashmap_s *hm);
void hashmap_finalize(hashmap_s *hm);
void hashmap_set(hashmap_s *hm, const char *key, void *value, size_t value_sizeof);
void* hashmap_get(hashmap_s *hm, const char *key, bool *found);

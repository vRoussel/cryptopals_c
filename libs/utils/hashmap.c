#include "hashmap.h"
#include "utils.h"

#include <assert.h>
#include <string.h>

static unsigned long hash(const unsigned char *str);

//TODO
// make it grow
// add delete
// make it generic ?

void hashmap_init(hashmap_s *hm)
{
    hm->buckets = calloc(HASHMAP_DEFAULT_BUCKET_COUNT, sizeof(node_s));
    hm->n_buckets = HASHMAP_DEFAULT_BUCKET_COUNT;
    hm->n_keys = 0;
}

void hashmap_finalize(hashmap_s *hm)
{
    free(hm->buckets);
}

void hashmap_set(hashmap_s *hm, const char *key, void *value, size_t value_sizeof)
{
    unsigned long key_hash = hash((const unsigned char *)key) % hm->n_buckets;
    node_s *prev_node = NULL;
    node_s *node = &(hm->buckets[key_hash]);
    if (!node->key) {
        node->key  = memdup(key, strlen(key) + 1);;
        if (value)
            node->value  = memdup(value, value_sizeof);
    } else {
        do {
            prev_node = node;
            node = node->next;
            if (!node) {
                node = calloc(1, sizeof(node_s));
                prev_node->next = node;
                break;
            }
            if (!node->key)
                break;
            if (strcmp(node->key, key) == 0)
                break;
        } while (true);

        if (!node->key)
            node->key  = memdup(key, strlen(key) + 1);;
        if (node->value)
            free(node->value);
        if (value)
            node->value  = memdup(value, value_sizeof);
    }
}

void* hashmap_get(hashmap_s *hm, const char *key, bool *found)
{
    void *ret = NULL;
    *found = false;
    unsigned long key_hash = hash((const unsigned char *)key) % hm->n_buckets;
    node_s *node = &(hm->buckets[key_hash]);
    while(node) {
        if (node->key && strcmp(node->key, key) == 0) {
            *found = true;
            ret = node->value;
        }
        node = node->next;
    }
    return ret;

}

// https://stackoverflow.com/a/7666577
static unsigned long hash(const unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

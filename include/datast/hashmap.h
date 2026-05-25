#ifndef HASHMAP_H
#define HASHMAP_H

#include <stddef.h>
#include <stdint.h>

typedef struct hashmap_t hashmap_t;

hashmap_t *hm_init(size_t capacity);
void hm_destroy(hashmap_t *hm);
void *hm_get(hashmap_t *hm, uint64_t key);
void hm_set(hashmap_t *hm, uint64_t key, void *value, void (*destructor)(void *));

#endif

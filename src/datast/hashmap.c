#include "datast/hashmap.h"
#include "datast/linkedlist.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

struct hashmap_t {
  size_t capacity;
  llist_t **hashmap;
};

typedef struct {
  uint64_t key;
  void *value;
  void (*destructor)(void *);
} hash_handler_t;

static uint64_t calculate_numerical_key(const char *key) {
  // Algoritmo de hash: djb2
  // http://www.cse.yorku.ca/~oz/hash.html

  uint64_t nkey = 5381;
  int c;

  while((c = *key++)) nkey = ((nkey << 5) + nkey) + c;

  return nkey;
}

static uint64_t hashing_algorithm(uint64_t value) {
  uint64_t hash = FNV_OFFSET;

  while(value) {
    uint64_t digit = value % 10;
    hash ^= digit;
    hash *= FNV_PRIME;
    value /= 10;
  }

  return hash;
}

static void hh_destroy(void *hh) {
  hash_handler_t *hh_local = hh;

  if (hh_local->destructor != NULL) hh_local->destructor(hh_local->value);
  free(hh);
}

hashmap_t *hm_init(size_t capacity) {
  hashmap_t *hm = malloc(sizeof(hashmap_t));
  if (hm == NULL) {
    printf("Erro na alocação de memória.\n");
    exit(1);
  }

  hm->capacity = capacity;
  hm->hashmap = calloc(capacity, sizeof(llist_t *));
  if (hm->hashmap == NULL) {
    printf("Erro na alocação de memória.\n");
    exit(1);
  }

  return hm;
}

void hm_destroy(hashmap_t *hm) {
  for (size_t i = 0; i < hm->capacity; i++) {
    if (hm->hashmap[i] != NULL) llist_destroy(hm->hashmap[i]);
  }

  free(hm->hashmap);
  free(hm);
}

void *hm_get(hashmap_t *hm, const char *key) {
  uint64_t nkey = calculate_numerical_key(key);
  uint64_t hash = hashing_algorithm(nkey);
  uint64_t index = hash % hm->capacity;

  llist_t *index_ll = hm->hashmap[index];

  if (index_ll == NULL) return NULL;

  node_t *current = llist_get_head(index_ll);
  size_t llist_len = llist_get_length(index_ll);
  for (size_t i = 0; i < llist_len; i++) {
    hash_handler_t *hh = node_get_value(current);

    if (hh->key == nkey) return hh->value;

    current = node_get_rpt(current);
  }

  return NULL;
}

void hm_set(hashmap_t *hm, const char *key, void *value, void (*destructor)(void *)) {
  hash_handler_t *hh = malloc(sizeof(hash_handler_t));
  if (hh == NULL) {
    printf("Erro na alocação de memória.\n");
    exit(1);
  }

  uint64_t nkey = calculate_numerical_key(key);

  hh->key = nkey;
  hh->value = value;
  hh->destructor = destructor;

  uint64_t hash = hashing_algorithm(nkey);
  uint64_t index = hash % hm->capacity;

  llist_t *index_ll = hm->hashmap[index];

  if (index_ll == NULL) {
    index_ll = llist_init();
    hm->hashmap[index] = index_ll;
  }

  llist_insertat_end(index_ll, node_init(hh, hh_destroy));
}

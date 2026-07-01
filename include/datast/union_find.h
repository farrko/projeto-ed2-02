#ifndef UNION_FIND_H
#define UNION_FIND_H

#include <stddef.h>
#include <stdbool.h>

typedef struct ufind_t ufind_t;

ufind_t *uf_init(size_t size);
void uf_unite(ufind_t *uf, size_t p, size_t q);
size_t uf_find(ufind_t *uf, size_t p);

#endif

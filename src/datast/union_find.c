#include "datast/union_find.h"
#include <stdlib.h>
#include <stdio.h>

struct ufind_t {
  size_t *id;
};

ufind_t *uf_init(size_t size) {
  ufind_t *uf = malloc(sizeof(ufind_t));
  if (uf == NULL) {
    printf("Erro na alocação de memória.\n");
    exit(1);
  }

  uf->id = malloc(size * sizeof(size_t));
  if (uf->id == NULL) {
    printf("Erro na alocação de memória.\n");
    exit(1);
  }

  for (size_t i = 0; i < size; i++) {
    uf->id[i] = i;
  }

  return uf;
}

void uf_unite(ufind_t *uf, size_t p, size_t q) {
  size_t root_p = uf_find(uf, p);
  size_t root_q = uf_find(uf, q);

  if (root_p == root_q) return;

  uf->id[root_p] = root_q;
}

size_t uf_find(ufind_t *uf, size_t p) {
  size_t root = p;

  while (root != uf->id[root]) {
    root = uf->id[root];
  }

  while (p != root) {
    size_t next = uf->id[p];
    uf->id[p] = root;
    p = next;
  }

  return root;
}

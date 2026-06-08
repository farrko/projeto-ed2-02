#include "datast/vector.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

struct vector_t {
  size_t size;
  size_t capacity;
  size_t data_size;
  void *data;
};

vector_t *vec_init(size_t data_size) {
  vector_t *v = malloc(sizeof(vector_t));
  if (!v) {
    printf("Erro na alocação de memória.\n");
    exit(1);
  }

  v->size = 0;
  v->capacity = 1;
  v->data_size = data_size;

  v->data = malloc(data_size);
  if (!v->data) {
    printf("Erro na alocação de memória.\n");
    exit(1);
  }

  return v;
}

void vec_destroy(vector_t *v) {
  free(v->data);
  free(v);
}

size_t vec_get_size(vector_t *v) {
  return v->size;
}

bool vec_is_empty(vector_t *v) {
  return v->size == 0 ? true : false;
}

void *vec_at(vector_t *v, size_t index) {
  if (index >= v->size || v->size == 0) return NULL;

  return (v->data + v->data_size * index);
}

void *vec_front(vector_t *v) {
  if (v->size == 0) return NULL;

  return (v->data);
}

void *vec_back(vector_t *v) {
  if (v->size == 0) return NULL;

  return (v->data + v->data_size * (v->size - 1));
}

bool vec_insert_at(vector_t *v, void *data, size_t index) {
  if (index > v->size) return false;
  if (index == v->size) {
    vec_push_back(v, data);
    return true;
  }

  uint8_t *pos = v->data + v->data_size * index;
  memcpy(pos, (uint8_t *) data, v->data_size);

  return true;
}

void vec_push_back(vector_t *v, void *data) {
  if (v->size == v->capacity) {
    v->capacity = v->capacity << 1;
    v->data = realloc(v->data, v->capacity * v->data_size);
    if (!v->data) {
      printf("Erro na alocação de memória.\n");
      exit (1);
    }
  }

  uint8_t *pos = v->data + v->data_size * v->size;
  memcpy(pos, (uint8_t *) data, v->data_size);
  v->size++;
}

void vec_pop_back(vector_t *v) {
  if (v->size == 0) return;

  v->size--;
}

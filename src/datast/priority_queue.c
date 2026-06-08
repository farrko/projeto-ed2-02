#include "datast/priority_queue.h"
#include "datast/vector.h"

#include <stdlib.h>
#include <stdio.h>

struct priqueue_t {
  vector_t *heap;
};

typedef struct {
  double priority;
  void *data;
} pq_data_t;

priqueue_t *pq_init() {
  priqueue_t *pq = malloc(sizeof(priqueue_t));
  if (pq == NULL) {
    printf("Erro na alocação de memória.\n");
    exit(1);
  }

  pq->heap = vec_init(sizeof(pq_data_t));

  return pq;
}

void pq_destroy(priqueue_t *pq) {
  vec_destroy(pq->heap);
  free(pq);
}

static void swap(vector_t *v, size_t i, size_t j) {
  pq_data_t tmp = *(pq_data_t *) vec_at(v, i);
  vec_insert_at(v, vec_at(v, j), i);
  vec_insert_at(v, &tmp, j);
}

static void shift_up(vector_t *heap, size_t index) {
  while (index > 0) {
    size_t pi = (index - 1) / 2;
    pq_data_t *child = vec_at(heap, index);
    pq_data_t *parent = vec_at(heap, pi);

    if (child->priority >= parent->priority) break;
    swap(heap, index, pi);
    index = pi;
  }
}

static void shift_down(vector_t *heap, size_t index) {
  size_t size = vec_get_size(heap);

  while (true) {
    size_t left = 2 * index + 1;
    size_t right = 2 * index + 2;
    size_t smallest = index;

    pq_data_t *s = vec_at(heap, smallest);

    if (left < size) {
      pq_data_t *l = vec_at(heap, left);
      if (l->priority < s->priority) {
        smallest = left;
        s = l;
      }
    }

    if (right < size) {
      pq_data_t *r = vec_at(heap, right);
      if (r->priority < s->priority) smallest = right;
    }

    if (smallest == index) break;
    swap(heap, index, smallest);
    index = smallest;
  }
}

void pq_enqueue(priqueue_t *pq, void *data, double priority) {
  pq_data_t entry = { priority, data };
  vec_push_back(pq->heap, &entry);
  shift_up(pq->heap, vec_get_size(pq->heap) - 1);
}

void *pq_dequeue(priqueue_t *pq) {
  if (vec_is_empty(pq->heap)) return NULL;

  pq_data_t *root = vec_front(pq->heap);
  void *res = root->data;

  vec_insert_at(pq->heap, vec_back(pq->heap), 0);
  vec_pop_back(pq->heap);

  if (!vec_is_empty(pq->heap)) shift_down(pq->heap, 0);

  return res;
}

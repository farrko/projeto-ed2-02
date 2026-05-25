#include "priority_queue/priority_queue.h"
#include "linkedlist/linkedlist.h"

#include <stdlib.h>
#include <stdio.h>

struct priqueue_t {
  llist_t *priority;
  llist_t *queue;
};

priqueue_t *pq_init() {
  priqueue_t *pq = malloc(sizeof(priqueue_t));
  if (pq == NULL) {
    printf("Erro na alocação de memória.\n");
    exit(1);
  }

  pq->priority = llist_init();
  pq->queue = llist_init();

  return pq;
}

void pq_destroy(priqueue_t *pq) {
  llist_destroy(pq->priority);
  llist_destroy(pq->queue);
  free(pq);
}

void pq_enqueue(priqueue_t *pq, node_t *node, double priority) {
  double *priority_p = malloc(sizeof(double));
   if (priority_p == NULL) {
    printf("Erro na alocação de memória.\n");
    exit(1);
  }

  *priority_p = priority;
  node_t *priority_node = node_init(priority_p, free);

  if (!llist_get_length(pq->priority)) {
    llist_insertat_start(pq->priority, priority_node);
    llist_insertat_start(pq->queue, node);

    return;
  }

  double priority_min = *((double *) node_get_value(llist_get_head(pq->priority)));
  double priority_max = *((double *) node_get_value(llist_get_tail(pq->priority)));

  if (priority <= priority_min) {
    llist_insertat_start(pq->priority, priority_node);
    llist_insertat_start(pq->queue, node);

    return;
  }

  if (priority >= priority_max) {
    llist_insertat_end(pq->priority, priority_node);
    llist_insertat_end(pq->queue, node);

    return;
  }

  node_t *current = llist_get_head(pq->priority);
  size_t llist_length = llist_get_length(pq->priority);

  for (size_t i = 0; i < llist_length; i++) {
    double current_priority = *((double *) node_get_value(current));

    if (priority > current_priority) {
      current = node_get_rpt(current);
      continue;
    }

    llist_insertat_index(pq->priority, priority_node, i);
    llist_insertat_index(pq->queue, node, i);

    return;
  }
}

node_t *pq_dequeue(priqueue_t *pq) {
  llist_destroyat_index(pq->priority, 0);
  return llist_popat_start(pq->queue);
}

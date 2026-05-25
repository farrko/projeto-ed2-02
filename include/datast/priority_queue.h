#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "linkedlist/nodes.h"

typedef struct priqueue_t priqueue_t;
typedef struct pq_data_t pq_data_t;

priqueue_t *pq_init();
void pq_destroy(priqueue_t *pq);

void pq_enqueue(priqueue_t *pq, node_t *node, double priority);
node_t *pq_dequeue(priqueue_t *pq);

#endif

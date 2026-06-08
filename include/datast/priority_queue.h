#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

typedef struct priqueue_t priqueue_t;

priqueue_t *pq_init();
void pq_destroy(priqueue_t *pq);

void pq_enqueue(priqueue_t *pq, void *data, double priority);
void *pq_dequeue(priqueue_t *pq);

#endif

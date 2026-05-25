#ifndef NODES_H
#define NODES_H

typedef struct node_t node_t;

node_t *node_init(void *value, void (*destructor)(void*));

void node_set_value(node_t *node, void *value);
void node_set_destructor(node_t *node, void (*destructor)(void *));
void node_set_lpt(node_t *node, node_t *lpt);
void node_set_rpt(node_t *node, node_t *rpt);

void *node_get_value(node_t *node);
node_t *node_get_lpt(node_t *node);
node_t *node_get_rpt(node_t *node);

void node_destroy(node_t *node);

#endif

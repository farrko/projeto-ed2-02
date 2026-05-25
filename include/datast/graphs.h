#ifndef GRAPHS_H
#define GRAPHS_H

#include "linkedlist/linkedlist.h"
#include <stdbool.h>

typedef struct graph_t graph_t;
typedef struct gnode_t gnode_t;
typedef struct connections_t connections_t;
typedef struct dijkstra_directions_t dijkstra_directions_t;

gnode_t *gnode_init(void *data, void (*destructor)(void *));
void *gnode_get_data(gnode_t *gnode);
llist_t *gnode_get_connections(gnode_t *gnode);
void gnode_destroy(void *gnode);

graph_t *graph_init();
llist_t *graph_get_nodes(graph_t *graph);
void graph_add_node(graph_t *graph, gnode_t *node);
void graph_add_edge(graph_t *graph, gnode_t *src, gnode_t *dst, double weight);
void graph_destroy(graph_t *graph);

gnode_t *conn_get_dst(connections_t *conn);
int conn_get_weight(connections_t *conn);

llist_t *graph_dijkstra(graph_t *graph, gnode_t *start, gnode_t *end);

gnode_t *dijd_get_node(dijkstra_directions_t *dijd);
gnode_t *dijd_get_from(dijkstra_directions_t *dijd);
double dijd_get_cost(dijkstra_directions_t *dijd);
bool dijd_get_visited(dijkstra_directions_t *dijd);

#endif

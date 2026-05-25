#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "graphs/graphs.h"
#include "linkedlist/linkedlist.h"
#include "linkedlist/nodes.h"
#include "priority_queue/priority_queue.h"
#include "hashmap/hashmap.h"

struct graph_t {
  llist_t *nodes;
};

struct gnode_t {
  void *data;
  void (*destructor)(void *);
  llist_t *connections;
};

struct connections_t {
  gnode_t *dst;
  double weight;
};

struct dijkstra_directions_t {
  gnode_t *node;
  gnode_t *from;
  double cost;
  bool visited;
};

gnode_t *gnode_init(void *data, void (*destructor)(void *)) {
  gnode_t *gnode = malloc(sizeof(gnode_t));
  if (gnode == NULL) {
    printf("Erro na alocação de memória.\n");
    exit(1);
  }

  gnode->data = data;
  gnode->destructor = destructor;
  gnode->connections = llist_init();

  return gnode;
}

void *gnode_get_data(gnode_t *gnode) {
  return gnode->data;
}

llist_t *gnode_get_connections(gnode_t *gnode) {
  return gnode->connections;
}

void gnode_destroy(void *gnode) {
  gnode_t *gn = (gnode_t *) gnode;

  if (gn->data != NULL) gn->destructor(gn->data);

  llist_destroy(gn->connections);
  free(gn);
}

graph_t *graph_init() {
  graph_t *graph = malloc(sizeof(graph_t));
  if (graph == NULL) {
    printf("Erro na alocação de memória.\n");
    exit(1);
  }

  graph->nodes = llist_init();
  return graph;
}

llist_t *graph_get_nodes(graph_t *graph) {
  return graph->nodes;
}

void graph_add_node(graph_t *graph, gnode_t *node) {
  llist_insertat_end(graph->nodes, node_init(node, gnode_destroy));
}

void graph_add_edge(graph_t *graph, gnode_t *src, gnode_t *dst, double weight) {
  llist_t *nodes = graph->nodes;
  node_t *current = llist_get_head(nodes);

  bool src_exists_in_graph = false;
  bool dst_exists_in_graph = false;

  for (size_t i = 0; i < llist_get_length(nodes); i++) {
    void *nval = node_get_value(current);
    if (nval == src) src_exists_in_graph = true;
    if (nval == dst) dst_exists_in_graph = true;

    if (src_exists_in_graph && dst_exists_in_graph) break;

    current = node_get_rpt(current);
  }

  if (!src_exists_in_graph || !dst_exists_in_graph) {
    printf("O gnode de source ou destination não está presente no grafo.\n");
    return;
  }

  connections_t *conn = malloc(sizeof(connections_t));
  if (conn == NULL) {
    printf("Erro na alocação de memória.\n");
    exit(1);
  }

  conn->dst = dst;
  conn->weight = weight;

  llist_insertat_end(src->connections, node_init(conn, free));
}

void graph_destroy(graph_t *graph) {
  llist_destroy(graph->nodes);
  free(graph);
}

gnode_t *conn_get_dst(connections_t *conn) {
  return conn->dst;
}

int conn_get_weight(connections_t *conn) {
  return conn->weight;
}

llist_t *graph_dijkstra(graph_t *graph, gnode_t *start, gnode_t *end) {
  priqueue_t *pq = pq_init();
  llist_t *final_shortest_distance = llist_init();

  node_t *graph_current = llist_get_head(graph->nodes);
  size_t graph_nodes_count = llist_get_length(graph->nodes);

  hashmap_t *di_hashmap = hm_init(graph_nodes_count);

  for (size_t i = 0; i < graph_nodes_count; i++) {
    dijkstra_directions_t *di = malloc(sizeof(dijkstra_directions_t));
    if (di == NULL) {
      printf("Erro na alocação de memória.\n");
      exit(1);
    }

    gnode_t *node = node_get_value(graph_current);

    di->node = node;
    di->from = start;
    di->cost = (start == node) ? 0 : INFINITY;
    di->visited = false;

    hm_set(di_hashmap, *((int *) node->data), di, free);

    graph_current = node_get_rpt(graph_current);
  }
  
  size_t start_conn_length = llist_get_length(start->connections);
  node_t *start_conn_current = llist_get_head(start->connections);

  for (size_t i = 0; i < start_conn_length; i++) {
    connections_t *conn = node_get_value(start_conn_current);

    dijkstra_directions_t *di = malloc(sizeof(dijkstra_directions_t));
    if (di == NULL) {
      printf("Erro na alocação de memória.\n");
      exit(1);
    }

    di->node = conn->dst;
    di->from = start;
    di->cost = conn->weight;
    di->visited = false;

    pq_enqueue(pq, node_init(di, free), conn->weight);
    start_conn_current = node_get_rpt(start_conn_current);
  }

  while(true) {
    node_t *node_from_pq = pq_dequeue(pq);
    if (node_from_pq == NULL) break;

    dijkstra_directions_t *from_pq = node_get_value(node_from_pq);
    free(node_from_pq);

    printf("FROM PRIORITY QUEUE:\n");
    printf("- Node: %d\n", *((int *) from_pq->node->data));
    printf("- From: %d\n", *((int *) from_pq->from->data));
    printf("- Cost: %f\n", from_pq->cost);

    dijkstra_directions_t *from_hm = hm_get(di_hashmap, *((int *) from_pq->node->data));

    if (from_hm == NULL) printf("from_hm é nulo\n");

    if (from_hm->visited) {
      free(from_pq);
      continue;
    }

    from_hm->from = from_pq->from;
    from_hm->cost = from_pq->cost;
    from_hm->visited = true;

    if (from_pq->node == end) {
      free(from_pq);
      break;
    }

    gnode_t *gnode = from_pq->node;
    node_t *conn_node = llist_get_head(gnode->connections);
    size_t conn_len = llist_get_length(gnode->connections);

    for (size_t i = 0; i < conn_len; i++) {
      connections_t *conn = node_get_value(conn_node);

      dijkstra_directions_t *di = malloc(sizeof(dijkstra_directions_t));
      if (di == NULL) {
        printf("Erro na alocação de memória.\n");
        exit(1);
      }

      di->node = conn->dst;
      di->from = from_pq->node;
      di->cost = from_pq->cost + conn->weight;
      di->visited = false;

      printf("INTO PRIORITY QUEUE:\n");
      printf("- Node: %d\n", *((int *) di->node->data));
      printf("- From: %d\n", *((int *) di->from->data));
      printf("- Cost: %f\n", di->cost);

      pq_enqueue(pq, node_init(di, free), di->cost);
      conn_node = node_get_rpt(conn_node);
    }

    free(from_pq);
  }

  dijkstra_directions_t *final_connection = hm_get(di_hashmap, *((int *) end->data));

  while(true) {
    dijkstra_directions_t *di = malloc(sizeof(dijkstra_directions_t));
    if (di == NULL) {
      printf("Erro na alocação de memória.\n");
      exit(1);
    }

    di->node = final_connection->node;
    di->from = final_connection->from;
    di->cost = final_connection->cost;
    di->visited = di->visited;

    llist_insertat_start(final_shortest_distance, node_init(di, free));

    if (final_connection->from == start) break;

    final_connection = hm_get(di_hashmap, *((int *) final_connection->from->data));
  }

  hm_destroy(di_hashmap);
  pq_destroy(pq);

  return final_shortest_distance;
}

gnode_t *dijd_get_node(dijkstra_directions_t *dijd) {
  return dijd->node;
}

gnode_t *dijd_get_from(dijkstra_directions_t *dijd) {
  return dijd->from;
}

double dijd_get_cost(dijkstra_directions_t *dijd) {
  return dijd->cost;
}

bool dijd_get_visited(dijkstra_directions_t *dijd) {
  return dijd->visited;
}

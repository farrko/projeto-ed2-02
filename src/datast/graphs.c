#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "datast/graphs.h"
#include "datast/priority_queue.h"
#include "datast/hashmap.h"
#include "datast/vector.h"
#include "utils.h"

struct graph_t {
  vector_t *nodes;
};

struct gnode_t {
  char *key;
  void *data;
  vector_t *connections;
};

typedef struct {
  gnode_t *dst;
  double distance;
  double cost;
} connections_t;

struct dijkstra_connections_t {
  gnode_t *node;
  gnode_t *from;
  double cost;
  bool visited;
};

gnode_t *gnode_init(const char *key, void *data) {
  gnode_t *gnode = malloc(sizeof(gnode_t));
  if (gnode == NULL) {
    printf("Erro na alocação de memória.\n");
    exit(1);
  }

  gnode->key = ns_strncpy(key, 100);
  gnode->data = data;
  gnode->connections = vec_init(sizeof(connections_t));

  return gnode;
}

const char *gnode_get_key(gnode_t *gnode) {
  return gnode->key;
}

void *gnode_get_data(gnode_t *gnode) {
  return gnode->data;
}

vector_t *gnode_get_connections(gnode_t *gnode) {
  return gnode->connections;
}

void gnode_destroy(gnode_t *gnode) {
  free(gnode->key);
  vec_destroy(gnode->connections);
  free(gnode);
}

graph_t *graph_init() {
  graph_t *graph = malloc(sizeof(graph_t));
  if (graph == NULL) {
    printf("Erro na alocação de memória.\n");
    exit(1);
  }

  graph->nodes = vec_init(sizeof(gnode_t *));

  return graph;
}

vector_t *graph_get_nodes(graph_t *graph) {
  return graph->nodes;
}

void graph_add_node(graph_t *graph, gnode_t *node) {
  vec_push_back(graph->nodes, &node);
}

void graph_add_edge(graph_t *graph, gnode_t *src, gnode_t *dst, double distance, double cost) {
  size_t size = vec_get_size(graph->nodes);

  bool src_exists_in_graph = false;
  bool dst_exists_in_graph = false;

  for (size_t i = 0; i < size; i++) {
    gnode_t *from_nodes = *(gnode_t **) vec_at(graph->nodes, i);

    if (from_nodes == src) src_exists_in_graph = true;
    if (from_nodes == dst) dst_exists_in_graph = true;

    if (src_exists_in_graph && dst_exists_in_graph) break;
  }

  if (!src_exists_in_graph || !dst_exists_in_graph) {
    printf("O gnode de source ou destination não está presente no grafo.\n");
    return;
  }

  connections_t conn = { dst, distance, cost };
  vec_push_back(src->connections, &conn);
}

graph_t *graph_to_undirected(graph_t *graph) {
  graph_t *undirected = graph_init();
  
  size_t nodes_size = vec_get_size(graph->nodes);
  hashmap_t *hm = hm_init(nodes_size * 2);

  for (size_t i = 0; i < nodes_size; i++) {
    gnode_t *ori = *(gnode_t **) vec_at(graph->nodes, i);
    gnode_t *cln = gnode_init(ori->key, ori->data);
    graph_add_node(undirected, cln);
    hm_set(hm, ori->key, cln, NULL);
  }

  for (size_t i = 0; i < nodes_size; i++) {
    gnode_t *ori = *(gnode_t **) vec_at(graph->nodes, i);
    gnode_t *cln = *(gnode_t **) vec_at(undirected->nodes, i);

    size_t conn_size = vec_get_size(ori->connections);
    for (size_t j = 0; j < conn_size; j++) {
      connections_t *c = vec_at(ori->connections, j);
      gnode_t *new_dst = hm_get(hm, c->dst->key);

      connections_t new_conn = { new_dst, c->distance, c->cost };
      vec_push_back(cln->connections, &new_conn);
    }
  }

  for (size_t i = 0; i < nodes_size; i++) {
    gnode_t *src = *(gnode_t **) vec_at(undirected->nodes, i);

    size_t conn_size = vec_get_size(src->connections);
    for (size_t j = 0; j < conn_size; j++) {
      connections_t *c = vec_at(src->connections, j);
      gnode_t *dst = c->dst;

      bool has_reverse = false;
      size_t dst_conn_n = vec_get_size(dst->connections);
      for (size_t k = 0; k < dst_conn_n; k++) {
        connections_t *dc = vec_at(dst->connections, k);

        if (dc->dst != src) continue;

        has_reverse = true;
        break;
      }

      if (has_reverse) continue;

      connections_t rev = { src, c->distance, c->cost };
      vec_push_back(dst->connections, &rev);
    }
  }
  
  hm_destroy(hm);
  return undirected;
}

void graph_destroy(graph_t *graph) {
  size_t size = vec_get_size(graph->nodes);
  for (size_t i = 0; i < size; i++) {
    gnode_destroy(*(gnode_t **)vec_at(graph->nodes, i));
  }

  vec_destroy(graph->nodes);
  free(graph);
}

static void swap(vector_t *v, size_t i, size_t j) {
  dijkstra_connections_t tmp = *(dijkstra_connections_t *) vec_at(v, i);
  vec_insert_at(v, vec_at(v, j), i);
  vec_insert_at(v, &tmp, j);
}

vector_t *graph_dijkstra(graph_t *graph, gnode_t *start, gnode_t *end, bool distance_or_cost) {
  priqueue_t *pq = pq_init();
  hashmap_t *di_hashmap = hm_init(vec_get_size(graph->nodes));

  for (size_t i = 0; i < vec_get_size(graph->nodes); i++) {
    gnode_t *node = *(gnode_t **) vec_at(graph->nodes, i);

    dijkstra_connections_t *di = malloc(sizeof(dijkstra_connections_t));

    di->node = node;
    di->from = start;
    di->cost = (start == node) ? 0 : INFINITY;
    di->visited = false;

    hm_set(di_hashmap, node->key, di, free);
  }

  for (size_t i = 0; i < vec_get_size(start->connections); i++) {
    connections_t *conn = vec_at(start->connections, i);

    dijkstra_connections_t *di = malloc(sizeof(dijkstra_connections_t));
    if (di == NULL) {
      printf("Erro na alocação de memória.\n");
      exit(1);
    }

    di->node = conn->dst;
    di->from = start;
    di->cost = distance_or_cost ? conn->distance : conn->cost;
    di->visited = false;

    pq_enqueue(pq, di, di->cost);
  }

  while(true) {
    dijkstra_connections_t *from_pq = pq_dequeue(pq);
    if (from_pq == NULL) break;

    dijkstra_connections_t *from_hm = hm_get(di_hashmap, from_pq->node->key);

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

    for (size_t i = 0; i < vec_get_size(from_pq->node->connections); i++) {
      connections_t *conn = vec_at(from_pq->node->connections, i);

      dijkstra_connections_t *di = malloc(sizeof(dijkstra_connections_t));
      if (di == NULL) {
        printf("Erro na alocação de memória.\n");
        exit(1);
      }

      di->node = conn->dst;
      di->from = from_pq->node;
      di->cost = from_pq->cost + (distance_or_cost ? conn->distance : conn->cost);
      di->visited = false;

      pq_enqueue(pq, di, di->cost);
    }

    free(from_pq);
  }

  dijkstra_connections_t *left_in_pq = pq_dequeue(pq);
  while (left_in_pq != NULL) {
    free(left_in_pq);
    left_in_pq = pq_dequeue(pq);
  }

  pq_destroy(pq);

  vector_t *final_shortest_distance = vec_init(sizeof(dijkstra_connections_t));
  dijkstra_connections_t *final_connection = hm_get(di_hashmap, end->key);

  while(true) {
    vec_push_back(final_shortest_distance, final_connection);
    if (final_connection->from == start) break;

    final_connection = hm_get(di_hashmap, final_connection->from->key);
  }

  size_t size = vec_get_size(final_shortest_distance);
  for (size_t i = 0; i < size; i++) {
    if (i >= size - i - 1) break;
    swap(final_shortest_distance, i, size - i - 1);
  }

  hm_destroy(di_hashmap);

  return final_shortest_distance;
}

gnode_t *dijc_get_node(dijkstra_connections_t *dijc) {
  return dijc->node;
}

gnode_t *dijc_get_from(dijkstra_connections_t *dijc) {
  return dijc->from;
}

double dijc_get_cost(dijkstra_connections_t *dijc) {
  return dijc->cost;
}

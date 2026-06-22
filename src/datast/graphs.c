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
  char *id;
  void *info;
  vector_t *connections;
};

struct edge_t {
  gnode_t *src;
  gnode_t *dst;
  void *info;
};

struct dijkstra_connections_t {
  gnode_t *node;
  gnode_t *from;
  edge_t *edge;
  double cost;
  bool visited;
};

gnode_t *gnode_init(const char *id, void *info) {
  gnode_t *gnode = malloc(sizeof(gnode_t));
  if (gnode == NULL) {
    printf("Erro na alocação de memória.\n");
    exit(1);
  }

  gnode->id = ns_strncpy(id, 100);
  gnode->info = info;
  gnode->connections = vec_init(sizeof(edge_t *));

  return gnode;
}

const char *gnode_get_id(gnode_t *gnode) {
  return gnode->id;
}

void *gnode_get_info(gnode_t *gnode) {
  return gnode->info;
}

vector_t *gnode_get_connections(gnode_t *gnode) {
  return gnode->connections;
}

void gnode_destroy(gnode_t *gnode) {
  size_t size = vec_get_size(gnode->connections);
  for (size_t i = 0; i < size; i++) {
    edge_t *edge = *(edge_t **) vec_at(gnode->connections, i);
    free(edge);
  }

  free(gnode->id);
  vec_destroy(gnode->connections);
  free(gnode);
}

gnode_t *edge_get_src(edge_t *edge) {
  return edge->src;
}

gnode_t *edge_get_dst(edge_t *edge) {
  return edge->dst;
}

void *edge_get_info(edge_t *edge) {
  return edge->info;
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

edge_t *graph_add_edge(graph_t *graph, gnode_t *src, gnode_t *dst, void *info) {
  size_t size = vec_get_size(graph->nodes);

  bool src_exists_in_graph = false;
  bool dst_exists_in_graph = false;

  for (size_t i = 0; i < size; i++) {
    gnode_t *node = *(gnode_t **) vec_at(graph->nodes, i);

    if (node == src) src_exists_in_graph = true;
    if (node == dst) dst_exists_in_graph = true;

    if (src_exists_in_graph && dst_exists_in_graph) break;
  }

  if (!src_exists_in_graph || !dst_exists_in_graph) {
    printf("O gnode de source ou destination não está presente no grafo.\n");
    return NULL;
  }

  edge_t *edge = malloc(sizeof(edge_t));
  if (edge == NULL) {
    printf("Erro na alocação de memória.\n");
    exit(1);
  }

  edge->src = src;
  edge->dst = dst;
  edge->info = info;

  vec_push_back(src->connections, &edge);

  return edge;
}

graph_t *graph_to_undirected(graph_t *graph) {
  graph_t *undirected = graph_init();

  size_t nodes_size = vec_get_size(graph->nodes);
  hashmap_t *hm = hm_init(nodes_size * 2);

  for (size_t i = 0; i < nodes_size; i++) {
    gnode_t *ori = *(gnode_t **) vec_at(graph->nodes, i);
    gnode_t *cln = gnode_init(ori->id, ori->info);
    graph_add_node(undirected, cln);
    hm_set(hm, ori->id, cln, NULL);
  }

  for (size_t i = 0; i < nodes_size; i++) {
    gnode_t *ori = *(gnode_t **) vec_at(graph->nodes, i);
    gnode_t *cln = *(gnode_t **) vec_at(undirected->nodes, i);

    size_t conn_size = vec_get_size(ori->connections);
    for (size_t j = 0; j < conn_size; j++) {
      edge_t *ori_edge = *(edge_t **) vec_at(ori->connections, j);
      gnode_t *new_dst = hm_get(hm, ori_edge->dst->id);

      graph_add_edge(undirected, cln, new_dst, ori_edge->info);
    }
  }

  for (size_t i = 0; i < nodes_size; i++) {
    gnode_t *src = *(gnode_t **) vec_at(undirected->nodes, i);

    size_t conn_size = vec_get_size(src->connections);
    for (size_t j = 0; j < conn_size; j++) {
      edge_t *edge = *(edge_t **) vec_at(src->connections, j);
      gnode_t *dst = edge->dst;

      bool has_reverse = false;
      size_t dst_conn_n = vec_get_size(dst->connections);
      for (size_t k = 0; k < dst_conn_n; k++) {
        edge_t *dst_edge = *(edge_t **) vec_at(dst->connections, k);

        if (dst_edge->dst != src) continue;

        has_reverse = true;
        break;
      }

      if (has_reverse) continue;

      graph_add_edge(undirected, dst, src, edge->info);
    }
  }

  hm_destroy(hm);
  return undirected;
}

void graph_destroy(graph_t *graph) {
  size_t size = vec_get_size(graph->nodes);
  for (size_t i = 0; i < size; i++) {
    gnode_destroy(*(gnode_t **) vec_at(graph->nodes, i));
  }

  vec_destroy(graph->nodes);
  free(graph);
}

static void swap(vector_t *v, size_t i, size_t j) {
  dijkstra_connections_t tmp = *(dijkstra_connections_t *) vec_at(v, i);
  vec_insert_at(v, vec_at(v, j), i);
  vec_insert_at(v, &tmp, j);
}

vector_t *graph_dijkstra(graph_t *graph, gnode_t *start, gnode_t *end, graph_weight_t weight_fn) {
  priqueue_t *pq = pq_init();
  hashmap_t *di_hashmap = hm_init(vec_get_size(graph->nodes));

  for (size_t i = 0; i < vec_get_size(graph->nodes); i++) {
    gnode_t *node = *(gnode_t **) vec_at(graph->nodes, i);

    dijkstra_connections_t *di = malloc(sizeof(dijkstra_connections_t));
    if (di == NULL) {
      printf("Erro na alocação de memória.\n");
      exit(1);
    }

    di->node = node;
    di->from = start;
    di->edge = NULL;
    di->cost = (start == node) ? 0 : INFINITY;
    di->visited = false;

    hm_set(di_hashmap, node->id, di, free);
  }

  size_t start_conn_size = vec_get_size(start->connections);
  for (size_t i = 0; i < start_conn_size; i++) {
    edge_t *edge = *(edge_t **) vec_at(start->connections, i);

    dijkstra_connections_t *di = malloc(sizeof(dijkstra_connections_t));
    if (di == NULL) {
      printf("Erro na alocação de memória.\n");
      exit(1);
    }

    di->node = edge->dst;
    di->from = start;
    di->edge = edge;
    di->cost = weight_fn(edge);
    di->visited = false;

    pq_enqueue(pq, di, di->cost);
  }

  while (true) {
    dijkstra_connections_t *from_pq = pq_dequeue(pq);
    if (from_pq == NULL) break;

    dijkstra_connections_t *from_hm = hm_get(di_hashmap, from_pq->node->id);

    if (from_hm->visited) {
      free(from_pq);
      continue;
    }

    from_hm->from = from_pq->from;
    from_hm->edge = from_pq->edge;
    from_hm->cost = from_pq->cost;
    from_hm->visited = true;

    if (from_pq->node == end) {
      free(from_pq);
      break;
    }

    size_t conn_size = vec_get_size(from_pq->node->connections);
    for (size_t i = 0; i < conn_size; i++) {
      edge_t *edge = *(edge_t **) vec_at(from_pq->node->connections, i);

      dijkstra_connections_t *di = malloc(sizeof(dijkstra_connections_t));
      if (di == NULL) {
        printf("Erro na alocação de memória.\n");
        exit(1);
      }

      di->node = edge->dst;
      di->from = from_pq->node;
      di->edge = edge;
      di->cost = from_pq->cost + weight_fn(edge);
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

  vector_t *final_shortest_path = vec_init(sizeof(dijkstra_connections_t));
  dijkstra_connections_t *final_connection = hm_get(di_hashmap, end->id);

  while (true) {
    vec_push_back(final_shortest_path, final_connection);
    if (final_connection->from == start) break;

    final_connection = hm_get(di_hashmap, final_connection->from->id);
  }

  size_t size = vec_get_size(final_shortest_path);
  for (size_t i = 0; i < size; i++) {
    if (i >= size - i - 1) break;
    swap(final_shortest_path, i, size - i - 1);
  }

  hm_destroy(di_hashmap);

  return final_shortest_path;
}

gnode_t *dijc_get_node(dijkstra_connections_t *dijc) {
  return dijc->node;
}

gnode_t *dijc_get_from(dijkstra_connections_t *dijc) {
  return dijc->from;
}

edge_t *dijc_get_edge(dijkstra_connections_t *dijc) {
  return dijc->edge;
}

double dijc_get_cost(dijkstra_connections_t *dijc) {
  return dijc->cost;
}

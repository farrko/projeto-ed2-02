#include "unity/unity.h"
#include "datast/graphs.h"
#include "datast/vector.h"
#include <math.h>

/*
 * Grafo utilizado nos testes de Dijkstra:
 *
 *   A --[d=1.0, c=10.0]--> B
 *   A --[d=5.0, c= 2.0]--> C
 *   C --[d=1.0, c= 1.0]--> B
 *
 * Caminho mínimo por distância: A -> B        (distância acumulada: 1.0)
 * Caminho mínimo por custo:     A -> C -> B   (custo acumulado: 3.0)
 */

#define DELTA 0.001

void setUp(void) {}
void tearDown(void) {}

static void build_test_graph(graph_t **graph, gnode_t **a, gnode_t **b, gnode_t **c) {
  *a = gnode_init("A", NULL);
  *b = gnode_init("B", NULL);
  *c = gnode_init("C", NULL);
  *graph = graph_init();
  graph_add_node(*graph, *a);
  graph_add_node(*graph, *b);
  graph_add_node(*graph, *c);
  graph_add_edge(*graph, *a, *b, 1.0, 10.0);
  graph_add_edge(*graph, *a, *c, 5.0,  2.0);
  graph_add_edge(*graph, *c, *b, 1.0,  1.0);
}

static void destroy_test_graph(graph_t *graph, gnode_t *a, gnode_t *b, gnode_t *c) {
  (void)a; (void)b; (void)c;
  graph_destroy(graph);
}
/* ── gnode ──────────────────────────────────────────────────────────────────── */

void test_gnode_init_e_getters(void) {
  int data = 42;
  gnode_t *node = gnode_init("meu_no", &data);
  TEST_ASSERT_NOT_NULL(node);
  TEST_ASSERT_EQUAL_STRING("meu_no", gnode_get_key(node));
  TEST_ASSERT_EQUAL_PTR(&data, gnode_get_data(node));
  gnode_destroy(node);
}

void test_gnode_conexoes_inicialmente_vazias(void) {
  gnode_t *node = gnode_init("X", NULL);
  vector_t *conns = gnode_get_connections(node);
  TEST_ASSERT_NOT_NULL(conns);
  TEST_ASSERT_EQUAL_size_t(0, vec_get_size(conns));
  gnode_destroy(node);
}

/* ── graph ──────────────────────────────────────────────────────────────────── */

void test_graph_init_sem_nos(void) {
  graph_t *graph = graph_init();
  TEST_ASSERT_NOT_NULL(graph);
  TEST_ASSERT_EQUAL_size_t(0, vec_get_size(graph_get_nodes(graph)));
  graph_destroy(graph);
}

void test_graph_add_node(void) {
  graph_t *graph = graph_init();
  gnode_t *node = gnode_init("A", NULL);
  graph_add_node(graph, node);
  TEST_ASSERT_EQUAL_size_t(1, vec_get_size(graph_get_nodes(graph)));
  graph_destroy(graph);
}

void test_graph_add_multiplos_nos(void) {
  graph_t *graph = graph_init();
  gnode_t *a = gnode_init("A", NULL);
  gnode_t *b = gnode_init("B", NULL);
  gnode_t *c = gnode_init("C", NULL);
  graph_add_node(graph, a);
  graph_add_node(graph, b);
  graph_add_node(graph, c);
  TEST_ASSERT_EQUAL_size_t(3, vec_get_size(graph_get_nodes(graph)));
  graph_destroy(graph);
}

void test_graph_add_edge_registra_conexao_em_src(void) {
  graph_t *graph = graph_init();
  gnode_t *a = gnode_init("A", NULL);
  gnode_t *b = gnode_init("B", NULL);
  graph_add_node(graph, a);
  graph_add_node(graph, b);
  graph_add_edge(graph, a, b, 1.0, 2.0);
  TEST_ASSERT_EQUAL_size_t(1, vec_get_size(gnode_get_connections(a)));
  graph_destroy(graph);
}

void test_graph_add_edge_nao_afeta_dst(void) {
  graph_t *graph = graph_init();
  gnode_t *a = gnode_init("A", NULL);
  gnode_t *b = gnode_init("B", NULL);
  graph_add_node(graph, a);
  graph_add_node(graph, b);
  graph_add_edge(graph, a, b, 1.0, 2.0);
  TEST_ASSERT_EQUAL_size_t(0, vec_get_size(gnode_get_connections(b)));
  graph_destroy(graph);
}

void test_graph_add_edge_destino_correto(void) {
  graph_t *graph = graph_init();
  gnode_t *a = gnode_init("A", NULL);
  gnode_t *b = gnode_init("B", NULL);
  graph_add_node(graph, a);
  graph_add_node(graph, b);
  graph_add_edge(graph, a, b, 1.0, 2.0);
  dijkstra_connections_t *conn = vec_at(gnode_get_connections(a), 0);
  TEST_ASSERT_EQUAL_PTR(b, dijc_get_node(conn));
  graph_destroy(graph);
}

/* ── dijkstra ───────────────────────────────────────────────────────────────── */

void test_dijkstra_retorna_vetor_nao_nulo(void) {
  graph_t *graph; gnode_t *a, *b, *c;
  build_test_graph(&graph, &a, &b, &c);
  vector_t *path = graph_dijkstra(graph, a, b, true);
  TEST_ASSERT_NOT_NULL(path);
  vec_destroy(path);
  destroy_test_graph(graph, a, b, c);
}

void test_dijkstra_caminho_por_distancia(void) {
  graph_t *graph; gnode_t *a, *b, *c;
  build_test_graph(&graph, &a, &b, &c);

  vector_t *path = graph_dijkstra(graph, a, b, true);
  TEST_ASSERT_EQUAL_size_t(1, vec_get_size(path));

  dijkstra_connections_t *entry = vec_at(path, 0);
  TEST_ASSERT_EQUAL_PTR(b, dijc_get_node(entry));
  TEST_ASSERT_DOUBLE_WITHIN(DELTA, 1.0, dijc_get_cost(entry));

  vec_destroy(path);
  destroy_test_graph(graph, a, b, c);
}

void test_dijkstra_caminho_por_custo(void) {
  graph_t *graph; gnode_t *a, *b, *c;
  build_test_graph(&graph, &a, &b, &c);

  vector_t *path = graph_dijkstra(graph, a, b, false);
  TEST_ASSERT_EQUAL_size_t(2, vec_get_size(path));

  dijkstra_connections_t *last = vec_back(path);
  TEST_ASSERT_EQUAL_PTR(b, dijc_get_node(last));
  TEST_ASSERT_DOUBLE_WITHIN(DELTA, 3.0, dijc_get_cost(last));

  vec_destroy(path);
  destroy_test_graph(graph, a, b, c);
}

void test_dijkstra_distancia_e_custo_produzem_caminhos_diferentes(void) {
  graph_t *graph; gnode_t *a, *b, *c;
  build_test_graph(&graph, &a, &b, &c);

  vector_t *by_dist = graph_dijkstra(graph, a, b, true);
  vector_t *by_cost = graph_dijkstra(graph, a, b, false);

  TEST_ASSERT_EQUAL_size_t(1, vec_get_size(by_dist));
  TEST_ASSERT_EQUAL_size_t(2, vec_get_size(by_cost));

  vec_destroy(by_dist);
  vec_destroy(by_cost);
  destroy_test_graph(graph, a, b, c);
}

void test_dijkstra_predecessores_corretos_por_custo(void) {
  graph_t *graph; gnode_t *a, *b, *c;
  build_test_graph(&graph, &a, &b, &c);

  vector_t *path = graph_dijkstra(graph, a, b, false);

  dijkstra_connections_t *step_c = vec_at(path, 0);
  dijkstra_connections_t *step_b = vec_at(path, 1);

  TEST_ASSERT_EQUAL_PTR(c, dijc_get_node(step_c));
  TEST_ASSERT_EQUAL_PTR(a, dijc_get_from(step_c));

  TEST_ASSERT_EQUAL_PTR(b, dijc_get_node(step_b));
  TEST_ASSERT_EQUAL_PTR(c, dijc_get_from(step_b));

  vec_destroy(path);
  destroy_test_graph(graph, a, b, c);
}

void test_dijkstra_no_inalcancavel_retorna_infinity(void) {
  graph_t *graph; gnode_t *a, *b, *c;
  build_test_graph(&graph, &a, &b, &c);
  gnode_t *isolated = gnode_init("D", NULL);
  graph_add_node(graph, isolated);

  vector_t *path = graph_dijkstra(graph, a, isolated, true);
  TEST_ASSERT_NOT_NULL(path);
  TEST_ASSERT_EQUAL_size_t(1, vec_get_size(path));

  dijkstra_connections_t *entry = vec_at(path, 0);
  TEST_ASSERT_EQUAL_PTR(isolated, dijc_get_node(entry));
  TEST_ASSERT_EQUAL_PTR(a,        dijc_get_from(entry));
  TEST_ASSERT_TRUE(isinf(dijc_get_cost(entry)));

  vec_destroy(path);
  destroy_test_graph(graph, a, b, c);
}

void test_dijkstra_start_igual_end(void) {
  graph_t *graph; gnode_t *a, *b, *c;
  build_test_graph(&graph, &a, &b, &c);

  vector_t *path = graph_dijkstra(graph, a, a, true);
  TEST_ASSERT_NOT_NULL(path);
  TEST_ASSERT_EQUAL_size_t(1, vec_get_size(path));

  dijkstra_connections_t *entry = vec_at(path, 0);
  TEST_ASSERT_EQUAL_PTR(a, dijc_get_node(entry));
  TEST_ASSERT_DOUBLE_WITHIN(DELTA, 0.0, dijc_get_cost(entry));

  vec_destroy(path);
  destroy_test_graph(graph, a, b, c);
}

/* ── main ───────────────────────────────────────────────────────────────────── */

int main(void) {
  UNITY_BEGIN();

  // gnode
  RUN_TEST(test_gnode_init_e_getters);
  RUN_TEST(test_gnode_conexoes_inicialmente_vazias);

  // graph
  RUN_TEST(test_graph_init_sem_nos);
  RUN_TEST(test_graph_add_node);
  RUN_TEST(test_graph_add_multiplos_nos);
  RUN_TEST(test_graph_add_edge_registra_conexao_em_src);
  RUN_TEST(test_graph_add_edge_nao_afeta_dst);
  RUN_TEST(test_graph_add_edge_destino_correto);

  // dijkstra
  RUN_TEST(test_dijkstra_retorna_vetor_nao_nulo);
  RUN_TEST(test_dijkstra_caminho_por_distancia);
  RUN_TEST(test_dijkstra_caminho_por_custo);
  RUN_TEST(test_dijkstra_distancia_e_custo_produzem_caminhos_diferentes);
  RUN_TEST(test_dijkstra_predecessores_corretos_por_custo);
  RUN_TEST(test_dijkstra_no_inalcancavel_retorna_infinity);
  RUN_TEST(test_dijkstra_start_igual_end);

  return UNITY_END();
}

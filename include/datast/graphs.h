#ifndef GRAPHS_H
#define GRAPHS_H

#include "datast/vector.h"
#include <stdbool.h>

/**
 * @file   graphs.h
 * @brief  Implementação de um grafo dirigido ponderado com suporte ao algoritmo de Dijkstra.
 *
 * O módulo graphs implementa um grafo dirigido cujas arestas carregam dois pesos
 * distintos: distância e custo. A distância representa a medida espacial entre dois
 * nós, enquanto o custo representa o esforço de locomoção entre eles, que pode
 * variar independentemente para o mesmo par de nós.
 *
 * O algoritmo de Dijkstra pode ser executado sobre qualquer um dos dois pesos,
 * controlado pelo parâmetro distance_or_cost. O resultado é retornado como um
 * vetor de dijkstra_connections_t, descrevendo o caminho mínimo do nó de origem
 * ao nó de destino.
 *
 * Cada nó (gnode_t) é identificado por uma chave textual única e pode carregar
 * um dado arbitrário via ponteiro opaco.
 */

typedef struct graph_t graph_t;
typedef struct gnode_t gnode_t;
typedef struct dijkstra_connections_t dijkstra_connections_t;

/** @brief    Inicializa um nó do grafo.
  *
  * @param    key   String que identifica unicamente o nó no grafo.
  * @param    data  Pointer para o dado arbitrário associado ao nó.
  *
  * @return   Uma instância de gnode_t pronta para uso.
  * @warning  Em caso de erro na alocação de memória, o programa será encerrado.
  */
gnode_t *gnode_init(const char *key, void *data);

/** @brief    Obtém a chave identificadora de um nó.
  *
  * @param    gnode  Pointer para o nó.
  *
  * @return   A string de identificação do nó.
  */
const char *gnode_get_key(gnode_t *gnode);

/** @brief    Obtém o dado associado a um nó.
  *
  * @param    gnode  Pointer para o nó.
  *
  * @return   Pointer para o dado armazenado no nó.
  */
void *gnode_get_data(gnode_t *gnode);

/** @brief    Obtém o vetor de conexões de saída de um nó.
  *
  * @param    gnode  Pointer para o nó.
  *
  * @return   Pointer para o vector_t contendo as arestas que partem deste nó.
  * @note     Os elementos do vetor retornado são do tipo dijkstra_connections_t.
  *           O vetor é de propriedade do nó e não deve ser destruído pelo chamador.
  */
vector_t *gnode_get_connections(gnode_t *gnode);

/** @brief    Destrói um nó e libera toda a memória associada a ele.
  *
  * @param    gnode  Pointer para o nó a ser destruído.
  *
  * @warning  O dado apontado por @p gnode não é liberado. O chamador é responsável
  *           por desalocar os dados associados antes de invocar esta função.
  * @warning  O nó não deve ser destruído enquanto ainda estiver inserido em um grafo.
  */
void gnode_destroy(gnode_t *gnode);

/** @brief    Inicializa um grafo dirigido vazio.
  *
  * @return   Uma instância de graph_t pronta para uso.
  * @warning  Em caso de erro na alocação de memória, o programa será encerrado.
  */
graph_t *graph_init();

/** @brief    Obtém o vetor de nós do grafo.
  *
  * @param    graph  Pointer para o grafo.
  *
  * @return   Pointer para o vector_t contendo todos os nós registrados no grafo.
  *           O vetor é de propriedade do grafo e não deve ser destruído pelo chamador.
  */
vector_t *graph_get_nodes(graph_t *graph);

/** @brief    Adiciona um nó ao grafo.
  *
  * @param    graph  Pointer para o grafo.
  * @param    node   Pointer para o nó a ser adicionado.
  *
  * @warning  O grafo não assume ownership do nó. O chamador é responsável por
  *           garantir que o nó permaneça válido enquanto estiver no grafo.
  */
void graph_add_node(graph_t *graph, gnode_t *node);

/** @brief    Adiciona uma aresta dirigida ponderada entre dois nós do grafo.
  *
  * @param    graph     Pointer para o grafo.
  * @param    src       Nó de origem da aresta.
  * @param    dst       Nó de destino da aresta.
  * @param    distance  Peso espacial da aresta, representando a distância entre os nós.
  * @param    cost      Peso de locomoção da aresta, representando o esforço ou tempo
  *                     necessário para percorrê-la. Pode diferir da distância para o
  *                     mesmo par de nós.
  *
  * @note     A aresta é dirigida: conecta @p src a @p dst, mas não o inverso.
  *           Para um grafo não-dirigido, chame esta função duas vezes invertendo src e dst.
  */
void graph_add_edge(graph_t *graph, gnode_t *src, gnode_t *dst, double distance, double cost);

/** @brief    Destrói um grafo e libera toda a memória associada a ele.
  *
  * @param    graph  Pointer para o grafo a ser destruído.
  *
  * @warning  Os nós e seus dados associados não são destruídos. O chamador é
  *           responsável por destruir cada gnode_t individualmente após esta chamada.
  */
void graph_destroy(graph_t *graph);

/** @brief    Executa o algoritmo de Dijkstra entre dois nós do grafo.
  *
  * @param    graph             Pointer para o grafo.
  * @param    start             Nó de origem do caminho.
  * @param    end               Nó de destino do caminho.
  * @param    distance_or_cost  Se @c true, minimiza a distância acumulada entre os nós.
  *                             Se @c false, minimiza o custo acumulado de locomoção.
  *
  * @return   Um vector_t de dijkstra_connections_t descrevendo o caminho mínimo de
  *           @p start até @p end, em ordem do início ao fim. Caso @p end seja
  *           inalcançável a partir de @p start, retorna um vetor de entrada única
  *           com @p start como origem, @p end como destino e custo igual a @c INFINITY.
  * @warning  O vetor retornado é sempre não-nulo e de propriedade do chamador;
  *           deve ser destruído após o uso com vec_destroy().
  */
vector_t *graph_dijkstra(graph_t *graph, gnode_t *start, gnode_t *end, bool distance_or_cost);

/** @brief    Obtém o nó de destino de uma entrada do resultado de Dijkstra.
  *
  * @param    dijc  Pointer para a entrada do caminho.
  *
  * @return   O nó ao qual esta entrada se refere.
  */
gnode_t *dijc_get_node(dijkstra_connections_t *dijc);

/** @brief    Obtém o nó predecessor de uma entrada do resultado de Dijkstra.
  *
  * @param    dijc  Pointer para a entrada do caminho.
  *
  * @return   O nó a partir do qual se chegou ao nó desta entrada, ou @c NULL
  *           se esta entrada corresponder ao nó de origem.
  */
gnode_t *dijc_get_from(dijkstra_connections_t *dijc);

/** @brief    Obtém o peso acumulado até o nó de uma entrada do resultado de Dijkstra.
  *
  * @param    dijc  Pointer para a entrada do caminho.
  *
  * @return   O valor acumulado de distância ou custo até este nó, conforme o critério
  *           utilizado na chamada a graph_dijkstra().
  */
double dijc_get_cost(dijkstra_connections_t *dijc);

#endif

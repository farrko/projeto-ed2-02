#ifndef GRAPHS_H
#define GRAPHS_H

#include "datast/vector.h"
#include <stdbool.h>

/**
 * @file   graphs.h
 * @brief  Implementação de um grafo dirigido genérico com suporte ao algoritmo de Dijkstra.
 *
 * O módulo graphs implementa um grafo dirigido cujos nós e conexões podem carregar uma
 * informação arbitrária por meio de um ponteiro opaco (void*), permitindo que o chamador
 * associe qualquer dado de domínio (por exemplo, atributos de uma rua) sem que o grafo
 * precise conhecer sua estrutura.
 *
 * O algoritmo de Dijkstra também é genérico: em vez de operar sobre campos fixos de peso,
 * ele recebe uma função (graph_weight_t) fornecida pelo chamador, que extrai de uma conexão
 * o valor numérico a ser acumulado e comparado ao longo do caminho. Isso permite calcular,
 * a partir do mesmo grafo, tanto o caminho de menor distância quanto o caminho mais rápido,
 * apenas alternando a função de peso utilizada.
 *
 * Cada nó (gnode_t) é identificado por um id textual único e carrega um dado arbitrário via
 * ponteiro opaco. Cada conexão (edge_t) liga um nó de origem a um nó de destino e também
 * carrega um dado arbitrário via ponteiro opaco.
 */

typedef struct graph_t graph_t;
typedef struct gnode_t gnode_t;
typedef struct edge_t edge_t;
typedef struct dijkstra_connections_t dijkstra_connections_t;

/** @brief    Função de comparação utilizada pelo algoritmo de Dijkstra.
  *
  * @param    edge  Conexão cujo peso deve ser calculado.
  *
  * @return   O valor numérico associado à conexão, utilizado para determinar qual
  *           caminho acumula o menor valor total (e, portanto, deve ser preferido).
  *
  * @note     É responsabilidade do chamador extrair, a partir da informação associada
  *           à conexão (ver edge_get_info()), o valor que deseja minimizar. Isso permite
  *           reutilizar o mesmo grafo tanto para o caminho de menor distância quanto para
  *           o caminho mais rápido, bastando fornecer funções de peso diferentes.
  */
typedef double (*graph_weight_t)(edge_t *edge);

/** @brief   Inicializa um nó do grafo.
 *
 * @param   id    String que identifica unicamente o nó no grafo.
 * @param   info  Pointer para o dado arbitrário associado ao nó.
 *
 * @return  Uma instância de gnode_t pronta para uso.
 * @warning Em caso de erro na alocação de memória, o programa será encerrado.
 */
gnode_t *gnode_init(const char *id, void *info);

/** @brief   Obtém o identificador de um nó.
 *
 * @param   gnode  Pointer para o nó.
 *
 * @return  A string de identificação do nó.
 */
const char *gnode_get_id(gnode_t *gnode);

/** @brief   Obtém o dado associado a um nó.
 *
 * @param   gnode  Pointer para o nó.
 *
 * @return  Pointer para o dado armazenado no nó.
 */
void *gnode_get_info(gnode_t *gnode);

/** @brief   Obtém o vetor de conexões de saída de um nó.
 *
 * @param   gnode  Pointer para o nó.
 *
 * @return  Pointer para o vector_t contendo as conexões (edge_t*) que partem deste nó.
 * @note    Os elementos do vetor retornado são do tipo edge_t*. O vetor é de propriedade
 *          do nó e não deve ser destruído pelo chamador.
 */
vector_t *gnode_get_connections(gnode_t *gnode);

/** @brief   Destrói um nó e libera toda a memória associada a ele, incluindo suas conexões.
 *
 * @param   gnode  Pointer para o nó a ser destruído.
 *
 * @warning O dado apontado por @p gnode e os dados apontados pelas conexões de saída deste
 *          nó não são liberados. O chamador é responsável por desalocar esses dados antes
 *          de invocar esta função.
 * @warning O nó não deve ser destruído enquanto ainda estiver inserido em um grafo.
 */
void gnode_destroy(gnode_t *gnode);

/** @brief   Obtém o nó de origem de uma conexão.
 *
 * @param   edge  Pointer para a conexão.
 *
 * @return  O nó a partir do qual a conexão parte.
 */
gnode_t *edge_get_src(edge_t *edge);

/** @brief   Obtém o nó de destino de uma conexão.
 *
 * @param   edge  Pointer para a conexão.
 *
 * @return  O nó ao qual a conexão chega.
 */
gnode_t *edge_get_dst(edge_t *edge);

/** @brief   Obtém o dado associado a uma conexão.
 *
 * @param   edge  Pointer para a conexão.
 *
 * @return  Pointer para o dado armazenado na conexão.
 */
void *edge_get_info(edge_t *edge);

/** @brief   Inicializa um grafo dirigido vazio.
 *
 * @return  Uma instância de graph_t pronta para uso.
 * @warning Em caso de erro na alocação de memória, o programa será encerrado.
 */
graph_t *graph_init();

/** @brief   Obtém o vetor de nós do grafo.
 *
 * @param   graph  Pointer para o grafo.
 *
 * @return  Pointer para o vector_t contendo todos os nós registrados no grafo. O vetor é
 *          de propriedade do grafo e não deve ser destruído pelo chamador.
 */
vector_t *graph_get_nodes(graph_t *graph);

/** @brief   Adiciona um nó ao grafo.
 *
 * @param   graph  Pointer para o grafo.
 * @param   node   Pointer para o nó a ser adicionado.
 *
 * @warning O grafo não assume ownership do nó. O chamador é responsável por garantir que
 *          o nó permaneça válido enquanto estiver no grafo.
 */
void graph_add_node(graph_t *graph, gnode_t *node);

/** @brief   Adiciona uma conexão dirigida entre dois nós do grafo.
 *
 * @param   graph  Pointer para o grafo.
 * @param   src    Nó de origem da conexão.
 * @param   dst    Nó de destino da conexão.
 * @param   info   Pointer para o dado arbitrário associado à conexão (por exemplo, os
 *                 atributos de um segmento de rua). Pode ser @c NULL.
 *
 * @return  Pointer para a conexão criada, de propriedade do nó de origem, ou @c NULL caso
 *          @p src ou @p dst não estejam presentes no grafo.
 * @note    A conexão é dirigida: liga @p src a @p dst, mas não o inverso. Para um grafo
 *          não-dirigido, chame esta função duas vezes invertendo src e dst.
 */
edge_t *graph_add_edge(graph_t *graph, gnode_t *src, gnode_t *dst, void *info);

/** @brief   Converte um grafo dirigido em um grafo não-dirigido equivalente.
 *
 * @param   graph  Pointer para o grafo dirigido de origem.
 *
 * @return  Pointer para um novo grafo não-dirigido, onde para cada conexão dirigida do
 *          grafo original, a conexão reversa também está presente. O grafo retornado é
 *          independente do original e deve ser destruído pelo chamador.
 *
 * @note    A conversão é feita em três etapas: clonagem dos nós, cópia das conexões com
 *          remapeamento dos ponteiros de destino, e inserção das conexões reversas
 *          ausentes. Conexões já mútuas no grafo original não são duplicadas.
 *
 * @warning Os dados associados a nós e conexões são copiados de forma superficial: o grafo
 *          resultante compartilha os mesmos ponteiros de informação do grafo original.
 */
graph_t *graph_to_undirected(graph_t *graph);

/** @brief   Destrói um grafo e libera toda a memória associada a ele.
 *
 * @param   graph  Pointer para o grafo a ser destruído.
 *
 * @warning Os nós, suas conexões e os dados associados a eles não são destruídos. O
 *          chamador é responsável por destruir cada gnode_t individualmente após esta
 *          chamada.
 */
void graph_destroy(graph_t *graph);

/** @brief   Executa o algoritmo de Dijkstra entre dois nós do grafo.
 *
 * @param   graph      Pointer para o grafo.
 * @param   start      Nó de origem do caminho.
 * @param   end        Nó de destino do caminho.
 * @param   weight_fn  Função de comparação que extrai, a partir de uma conexão, o valor
 *                     numérico a ser acumulado e minimizado ao longo do caminho (ver
 *                     graph_weight_t).
 *
 * @return  Um vector_t de dijkstra_connections_t descrevendo o caminho mínimo de @p start
 *          até @p end, em ordem do início ao fim. Caso @p end seja inalcançável a partir
 *          de @p start, retorna um vetor de entrada única com @p start como origem, @p end
 *          como destino, conexão nula e custo igual a @c INFINITY.
 * @warning O vetor retornado é sempre não-nulo e de propriedade do chamador; deve ser
 *          destruído após o uso com vec_destroy().
 */
vector_t *graph_dijkstra(graph_t *graph, gnode_t *start, gnode_t *end, graph_weight_t weight_fn);

/** @brief   Obtém o nó de destino de uma entrada do resultado de Dijkstra.
 *
 * @param   dijc  Pointer para a entrada do caminho.
 *
 * @return  O nó ao qual esta entrada se refere.
 */
gnode_t *dijc_get_node(dijkstra_connections_t *dijc);

/** @brief   Obtém o nó predecessor de uma entrada do resultado de Dijkstra.
 *
 * @param   dijc  Pointer para a entrada do caminho.
 *
 * @return  O nó a partir do qual se chegou ao nó desta entrada, ou @c NULL se esta entrada
 *          corresponder ao nó de origem.
 */
gnode_t *dijc_get_from(dijkstra_connections_t *dijc);

/** @brief   Obtém a conexão utilizada pelo caminho para alcançar o nó desta entrada.
 *
 * @param   dijc  Pointer para a entrada do caminho.
 *
 * @return  A conexão (edge_t*) percorrida para chegar ao nó desta entrada, ou @c NULL se
 *          esta entrada corresponder ao nó de origem.
 */
edge_t *dijc_get_edge(dijkstra_connections_t *dijc);

/** @brief   Obtém o custo acumulado até o nó de uma entrada do resultado de Dijkstra.
 *
 * @param   dijc  Pointer para a entrada do caminho.
 *
 * @return  O valor acumulado ao longo do caminho até este nó, de acordo com a função de
 *          peso (graph_weight_t) utilizada na chamada a graph_dijkstra().
 */
double dijc_get_cost(dijkstra_connections_t *dijc);

#endif

#ifndef GRAPHS_H
#define GRAPHS_H

#include "datast/vector.h"
#include "datast/hashmap.h"
#include <stdbool.h>

/**
 * @file   graphs.h
 * @brief  Implementação de um grafo dirigido genérico, owning, com suporte ao algoritmo
 *         de Dijkstra.
 *
 * O módulo graphs implementa um grafo dirigido cujos nós e conexões podem carregar uma
 * informação arbitrária por meio de um ponteiro opaco (void*). O grafo pode opcionalmente
 * assumir ownership sobre essas informações: ao ser inicializado com funções de clonagem e
 * destruição (ver graph_init()), ele passa a ser responsável por liberá-las em
 * graph_destroy() e por duplicá-las em graph_clone()/graph_to_undirected(), dispensando o
 * chamador de varrer manualmente nós e conexões para isso.
 *
 * Caso o grafo não receba essas funções (todos os parâmetros NULL em graph_init()), ele se
 * comporta como uma estrutura não-owning: nenhuma informação é liberada ou duplicada
 * automaticamente, e o chamador permanece responsável pelo ciclo de vida desses dados.
 *
 * O grafo também mantém, sob demanda, um índice (hashmap) de id para gnode_t*, utilizado
 * para acelerar buscas externas por id e a própria verificação de pertencimento em
 * graph_add_edge(). Este índice não é atualizado automaticamente quando novos nós são
 * adicionados; cabe ao chamador regerá-lo explicitamente quando necessário (ver
 * graph_generate_node_index() e graph_get_node_index()).
 *
 * O algoritmo de Dijkstra é genérico quanto ao peso utilizado: em vez de operar sobre
 * campos fixos, ele recebe uma função (graph_weight_t) fornecida pelo chamador, que extrai
 * de uma conexão o valor numérico a ser acumulado e comparado ao longo do caminho. Isso
 * permite calcular, a partir do mesmo grafo, tanto o caminho de menor distância quanto o
 * caminho mais rápido, apenas alternando a função de peso utilizada.
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
  * @note     É responsabilidade do chamador extrair, a partir da informação associada à
  *           conexão (ver edge_get_info()), o valor que deseja minimizar. Isso permite
  *           reutilizar o mesmo grafo tanto para o caminho de menor distância quanto para
  *           o caminho mais rápido, bastando fornecer funções de peso diferentes.
  */
typedef double (*graph_weight_t)(edge_t *edge);

typedef int (*edge_cmp_t)(const void *, const void *);

/** @brief   Função de clonagem de uma informação associada a um nó ou a uma conexão.
 *
 * @param   info  Pointer para a informação original a ser clonada.
 *
 * @return  Pointer para uma nova cópia independente da informação.
 *
 * @note    Fornecida ao grafo em graph_init(). Caso o grafo não assuma ownership sobre a
 *          informação correspondente, pode ser @c NULL.
 */
typedef void *(*graph_info_clone_t)(const void *info);

/** @brief   Função de destruição de uma informação associada a um nó ou a uma conexão.
 *
 * @param   info  Pointer para a informação a ser destruída.
 *
 * @note    Fornecida ao grafo em graph_init(). Caso o grafo não assuma ownership sobre a
 *          informação correspondente, pode ser @c NULL.
 */
typedef void (*graph_info_destroy_t)(void *info);

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

/** @brief   Destrói um nó e libera a memória de sua estrutura, incluindo as conexões de
 *           saída (mas não a informação associada ao nó nem às conexões).
 *
 * @param   gnode  Pointer para o nó a ser destruído.
 *
 * @warning O dado apontado por @p gnode e os dados apontados pelas conexões de saída deste
 *          nó não são liberados por esta função; caso o nó esteja inserido em um grafo
 *          owning, prefira destruí-lo por meio de graph_destroy().
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
 * @param   node_clone    Função utilizada para clonar a informação associada a nós em
 *                         graph_clone() e graph_to_undirected(). Pode ser @c NULL caso o
 *                         grafo não precise suportar clonagem de nós.
 * @param   node_destroy  Função utilizada para liberar a informação associada a nós em
 *                         graph_destroy(). Pode ser @c NULL caso o grafo não assuma
 *                         ownership sobre a informação dos nós.
 * @param   edge_clone    Função utilizada para clonar a informação associada a conexões em
 *                         graph_clone() e graph_to_undirected(). Pode ser @c NULL caso o
 *                         grafo não precise suportar clonagem de conexões.
 * @param   edge_destroy  Função utilizada para liberar a informação associada a conexões
 *                         em graph_destroy(). Pode ser @c NULL caso o grafo não assuma
 *                         ownership sobre a informação das conexões.
 *
 * @return  Uma instância de graph_t pronta para uso.
 * @warning Em caso de erro na alocação de memória, o programa será encerrado.
 * @warning Caso uma função de destruição seja fornecida sem a correspondente função de
 *          clonagem, graph_clone() e graph_to_undirected() recusarão operar sobre o grafo
 *          resultante (retornando @c NULL), para evitar liberação dupla da mesma
 *          informação por dois grafos distintos.
 */
graph_t *graph_init(graph_info_clone_t node_clone, graph_info_destroy_t node_destroy,
                     graph_info_clone_t edge_clone, graph_info_destroy_t edge_destroy);

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
 * @note    Esta operação não atualiza o índice de nós eventualmente já gerado (ver
 *          graph_get_node_index()); regere-o explicitamente após adicionar novos nós, caso
 *          deseje que buscas subsequentes os encontrem através do índice.
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
 * @note    Caso o índice de nós (ver graph_get_node_index()) exista e esteja sincronizado
 *          com a quantidade atual de nós do grafo, ele é utilizado para verificar o
 *          pertencimento de @p src e @p dst em tempo constante; caso contrário, é feita uma
 *          varredura linear sobre todos os nós do grafo.
 */
edge_t *graph_add_edge(graph_t *graph, gnode_t *src, gnode_t *dst, void *info);

/** @brief   Converte um grafo dirigido em um grafo não-dirigido equivalente.
 *
 * @param   graph          Pointer para o grafo dirigido de origem.
 * @param   shared_edges   Se @c true, a informação associada a cada conexão (incluindo as
 *                          conexões reversas sintetizadas) não é clonada: o grafo retornado
 *                          passa a compartilhar os mesmos ponteiros de @p graph. Se
 *                          @c false, mantém o comportamento original de clonagem profunda.
 *
 * @return  Pointer para um novo grafo não-dirigido, com as mesmas funções de clonagem do
 *          grafo original, onde para cada conexão dirigida do grafo original a conexão
 *          reversa também está presente. Retorna @c NULL caso o grafo possua ownership
 *          sobre a informação das conexões sem a correspondente função de clonagem (ver
 *          graph_init()). O grafo retornado é independente do original quanto aos nós, e
 *          deve ser destruído pelo chamador.
 *
 * @note    A conversão é feita em três etapas: clonagem dos nós, cópia das conexões com
 *          remapeamento dos ponteiros de destino, e inserção das conexões reversas
 *          ausentes. Conexões já mútuas no grafo original não são duplicadas.
 * @warning Quando @p shared_edges for @c true, o grafo retornado é inicializado sem função
 *          de destruição de conexões (mesmo que @p graph possua uma), para evitar liberação
 *          da informação de conexão enquanto @p graph ainda a possui. O chamador deve
 *          garantir que o grafo retornado seja destruído antes de @p graph.
 */
graph_t *graph_to_undirected(graph_t *graph, bool shared_edges);

/** @brief   Clona um grafo, criando uma cópia profunda independente.
 *
 * @param   graph  Pointer para o grafo a ser clonado.
 *
 * @return  Pointer para um novo grafo, com os mesmos nós e conexões do original, onde a
 *          informação de cada nó e de cada conexão foi duplicada por meio das funções de
 *          clonagem fornecidas em graph_init(). Caso uma das funções de clonagem não tenha
 *          sido fornecida enquanto a correspondente função de destruição foi, retorna
 *          @c NULL (ver graph_init()). O grafo retornado é independente do original e deve
 *          ser destruído pelo chamador.
 */
graph_t *graph_clone(graph_t *graph);

/** @brief   Destrói um grafo, liberando toda a memória de sua estrutura interna.
 *
 * @param   graph  Pointer para o grafo a ser destruído.
 *
 * @note    Caso o grafo tenha sido inicializado com funções de destruição (ver
 *          graph_init()), a informação associada a cada nó e a cada conexão também é
 *          liberada por meio dessas funções. Caso contrário, nenhuma informação é liberada
 *          e o chamador permanece responsável por ela.
 */
void graph_destroy(graph_t *graph);

/** @brief   Gera (ou regenera) o índice de busca de nós por id, descartando o índice
 *           anterior, caso exista.
 *
 * @param   graph  Pointer para o grafo.
 *
 * @return  Pointer para o índice (hashmap_t) gerado, contendo todos os nós atualmente
 *          presentes no grafo, mapeados por id. O índice é de propriedade do grafo e não
 *          deve ser destruído pelo chamador.
 * @note    Chame esta função explicitamente após adicionar novos nós ao grafo, caso deseje
 *          que o índice reflita essa mudança; graph_get_node_index() não regera o índice
 *          automaticamente.
 */
hashmap_t *graph_generate_node_index(graph_t *graph);

/** @brief   Obtém o índice de busca de nós por id, gerando-o caso ainda não exista.
 *
 * @param   graph  Pointer para o grafo.
 *
 * @return  Pointer para o índice (hashmap_t) atualmente associado ao grafo. O índice é de
 *          propriedade do grafo e não deve ser destruído pelo chamador.
 * @warning Caso nós tenham sido adicionados ao grafo após a última geração do índice, eles
 *          não estarão presentes no índice retornado; utilize graph_generate_node_index()
 *          para refleti-los.
 */
hashmap_t *graph_get_node_index(graph_t *graph);

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

/** @brief    Calcula a árvore geradora mínima de um grafo não-dirigido, selecionando
  *           apenas as conexões cujo peso (ver @p weight_fn) seja inferior a um limite.
  *
  * @param    graph      Pointer para o grafo não-dirigido de origem (ver
  *                       graph_to_undirected()).
  * @param    cmp_fn     Função de comparação utilizada para ordenar as conexões por peso
  *                       antes da construção da árvore (ver edge_cmp_t).
  * @param    weight_fn  Função que extrai, de uma conexão já pertencente à árvore geradora
  *                       mínima, o valor a ser comparado contra @p limiter para decidir sua
  *                       inclusão no grafo retornado.
  * @param    limiter    Valor limite: conexões com @p weight_fn maior ou igual a este valor
  *                       não são incluídas no grafo retornado, mesmo pertencendo à árvore.
  *
  * @return   Um novo graph_t contendo todos os nós do grafo original e apenas as conexões
  *           da árvore geradora mínima cujo peso (ver @p weight_fn) seja inferior a
  *           @p limiter. Retorna @c NULL nas mesmas condições de ownership descritas em
  *           graph_clone(). O grafo retornado é independente do original e deve ser
  *           destruído pelo chamador.
  * @note     A topologia da árvore é decidida considerando todas as conexões do grafo,
  *           independentemente de @p limiter; o limite apenas filtra quais conexões, já
  *           parte da árvore, aparecem no grafo retornado.
  * @warning  A informação associada às conexões do grafo retornado não é clonada: é
  *           compartilhada com @p graph. O grafo retornado deve ser destruído antes de
  *           @p graph, para evitar acesso a memória já liberada.
  */
graph_t *graph_kruskal(graph_t *graph, edge_cmp_t cmp_fn, graph_weight_t weight_fn, double limiter);

#endif

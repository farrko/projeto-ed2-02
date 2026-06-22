#ifndef VIA_H
#define VIA_H

#include "datast/graphs.h"

/*
 *  VIA
 *
 * O módulo VIA é responsável pelo processamento de um arquivo .via, que possui a
 * modelagem do sistema viário de uma cidade como um grafo direcionado. Cada vértice do
 * arquivo corresponde a uma esquina (extremo de um segmento de rua) e é representado por
 * um gnode_t cuja informação associada é um point_t com sua coordenada [x,y]. Cada aresta
 * do arquivo corresponde a um segmento de rua dirigido e é representada por um edge_t cuja
 * informação associada é um street_t, contendo nome, comprimento, velocidade média e as
 * chaves de CEP das quadras lindeiras a cada lado do segmento.
 *
 * O grafo retornado assume ownership sobre toda a informação associada a seus vértices e
 * arestas: destruí-lo com graph_destroy() é suficiente para liberar também os point_t e
 * street_t internos, sem necessidade de uma função de destruição específica deste módulo.
 *
 * A primeira linha do arquivo, que indica o número de vértices, não é utilizada: o grafo
 * dimensiona seu próprio índice de busca por id (ver graph_generate_node_index()) a partir
 * da contagem real de vértices já adicionados, no momento em que a primeira aresta é lida.
 */

/** @brief    Processa um arquivo .via e constrói o grafo do sistema viário correspondente.
  *
  * @param    viapath  Caminho do arquivo .via a ser processado.
  *
  * @return   Uma instância de graph_t representando o sistema viário, com ownership sobre
  *           a informação de seus vértices (point_t) e arestas (street_t).
  * @warning  Em caso de erro na leitura do arquivo, o programa será encerrado.
  * @warning  Arestas que referenciem vértices não declarados anteriormente no arquivo são
  *           ignoradas.
  */
graph_t *via_processing(const char *viapath);

#endif

#ifndef PATH_H
#define PATH_H

#include <stddef.h>
#include "shapes/point.h"

/*
 *  CAMINHO (PATH)
 *
 * O módulo de caminho (path) define a estrutura path_t, responsável por representar uma
 * sequência ordenada de N pontos (X -> Y -> ... -> Z), sem quantidade fixa definida em tempo
 * de declaração. Cada ponto é armazenado por valor em um vetor interno, e a ordem de inserção
 * determina a ordem de desenho do caminho.
 *
 * O módulo destina-se à representação visual de trajetos, como o caminho resultante do
 * algoritmo de Dijkstra sobre o grafo do sistema viário, sendo escrito como uma sequência de
 * segmentos de reta no arquivo SVG.
 *
 * A estrutura pode ser criada por meio de path_init() e destruída com path_destroy(). Pontos
 * são adicionados ao final do caminho com path_add_point().
 */

typedef struct path_t path_t;

/** @brief    Inicializa um caminho vazio.
  *
  * @param    id      Identificador do objeto.
  * @param    color   String com o código HEX da cor de preenchimento do caminho.
  *
  * @return   Uma instância de path_t pronta para uso.
  * @warning  Em caso de erro na alocação de memória, o programa será encerrado.
  */
path_t *path_init(size_t id, const char *color);

/** @brief    Destrói um caminho, liberando a memória de seus pontos internos.
  *
  * @param    path  Uma instância de caminho.
  */
void path_destroy(void *path);

/** @brief    Adiciona um ponto ao final do caminho.
  *
  * @param    path  Uma instância de caminho.
  * @param    x     Coordenada X do ponto a ser adicionado.
  * @param    y     Coordenada Y do ponto a ser adicionado.
  */
void path_add_point(path_t *path, double x, double y);

/** @brief    Retorna o ID de um caminho.
  *
  * @param    path  Uma instância de caminho.
  *
  * @return   O ID do caminho.
  */
size_t path_get_id(path_t *path);

/** @brief    Retorna a cor de preenchimento de um caminho.
  *
  * @param    path  Uma instância de caminho.
  *
  * @return   Uma string com a cor de preenchimento do caminho.
  */
const char *path_get_color(path_t *path);

/** @brief    Retorna o número de pontos presentes no caminho.
  *
  * @param    path  Uma instância de caminho.
  *
  * @return   A quantidade de pontos armazenados no caminho.
  */
size_t path_get_size(path_t *path);

/** @brief    Acessa o ponto na posição especificada do caminho.
  *
  * @param    path   Uma instância de caminho.
  * @param    index  Índice do ponto desejado.
  *
  * @return   Pointer para o ponto na posição @p index, ou @c NULL se o índice estiver
  *           fora dos limites do caminho.
  */
point_t *path_get_point(path_t *path, size_t index);

#endif

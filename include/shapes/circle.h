#ifndef CIRCLE_H
#define CIRCLE_H

#include <stddef.h>
#include "shapes/point.h"

/*
 *  CÍRCULO
 *
 * O módulo de círculo (circle) define a estrutura circle_t, responsável por representar
 * um círculo como forma geométrica. Cada círculo é identificado por um ID único e possui
 * posição (x, y) do seu centro, raio, e propriedades visuais de cor de preenchimento e
 * cor de borda para fins de exibição.
 *
 * A estrutura pode ser criada por meio de circle_init() e destruída com circle_destroy().
 * São disponibilizadas operações de leitura (get) e escrita (set) para todos os campos.
 */

typedef struct circle_t circle_t;

/** @brief    Inicialização de um círculo.
  *
  * @param    id              Identificador do objeto.
  * @param    x               Coordenada X do centro.
  * @param    y               Coordenada Y do centro.
  * @param    radius          Raio do círculo.
  * @param    color           String com o código HEX da cor de preenchimento.
  * @param    border_color    String com o código HEX da cor da borda.
  *
  * @return   Uma instância de círculo.
  * @warning  Em caso de erro na alocação de memória, o programa será encerrado.
  */
circle_t *circle_init(size_t id, double x, double y, double radius, char *color, char *border_color);

/** @brief    Destrói um círculo.
  *
  * @param    circle  Uma instância de círculo.
  */
void circle_destroy(void *circle);

/** @brief    Define o X de um círculo.
  *
  * @param    circle  Uma instância de círculo.
  * @param    x       Coordenada X do centro.
  */
void circle_set_x(circle_t *circle, double x);

/** @brief    Define o Y de um círculo.
  *
  * @param    circle  Uma instância de círculo.
  * @param    y       Coordenada Y do centro.
  */
void circle_set_y(circle_t *circle, double y);

/** @brief    Define o raio de um círculo.
  *
  * @param    circle  Uma instância de círculo.
  * @param    radius  Raio do círculo.
  */
void circle_set_radius(circle_t *circle, double radius);

/** @brief    Define a cor de preenchimento de um círculo.
  *
  * @param    circle  Uma instância de círculo.
  * @param    color   String com o código HEX da cor de preenchimento.
  */
void circle_set_color(circle_t *circle, char *color);

/** @brief    Define a cor da borda de um círculo.
  *
  * @param    circle        Uma instância de círculo.
  * @param    border_color  String com o código HEX da cor da borda.
  */
void circle_set_border_color(circle_t *circle, char *border_color);

/** @brief    Retorna o ID de um círculo.
  *
  * @param    circle  Uma instância de círculo.
  *
  * @return   O ID de um círculo.
  */
size_t circle_get_id(circle_t *circle);

/** @brief    Obtém o ponto de origem (centro) de um círculo.
  *
  * @param    circle Pointer para um círculo.
  *
  * @return   O ponto que representa o centro do círculo.
  */
point_t *circle_get_origin(circle_t *circle);

/** @brief    Retorna o X de um círculo.
  *
  * @param    circle  Uma instância de círculo.
  *
  * @return   O X do centro do círculo.
  */
double circle_get_x(circle_t *circle);

/** @brief    Retorna o Y de um círculo.
  *
  * @param    circle  Uma instância de círculo.
  *
  * @return   O Y do centro do círculo.
  */
double circle_get_y(circle_t *circle);

/** @brief    Retorna o raio de um círculo.
  *
  * @param    circle  Uma instância de círculo.
  *
  * @return   O raio do círculo.
  */
double circle_get_radius(circle_t *circle);

/** @brief    Retorna a cor de preenchimento de um círculo.
  *
  * @param    circle  Uma instância de círculo.
  *
  * @return   Uma string com a cor de preenchimento do círculo.
  */
char *circle_get_color(circle_t *circle);

/** @brief    Retorna a cor da borda de um círculo.
  *
  * @param    circle  Uma instância de círculo.
  *
  * @return   Uma string com a cor da borda do círculo.
  */
char *circle_get_border_color(circle_t *circle);

#endif

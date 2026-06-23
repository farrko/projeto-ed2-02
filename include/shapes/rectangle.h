#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <stddef.h>
#include "shapes/point.h"

/*
 *  RETÂNGULO
 *
 * O módulo de retângulo (rectangle) define a estrutura rectangle_t, responsável por representar
 * um retângulo como forma geométrica. Cada retângulo é identificado por um ID único e possui
 * posição (x, y) do seu canto superior esquerdo, dimensões (largura e altura), e propriedades
 * visuais de cor de preenchimento e cor de borda para fins de exibição.
 *
 * A estrutura pode ser criada por meio de rect_init() e destruída com rect_destroy().
 * São disponibilizadas operações de leitura (get) e escrita (set) para todos os campos.
 */

typedef struct rectangle_t rectangle_t;

/** @brief    Inicialização de um retângulo.
  *
  * @param    id              Identificador do objeto.
  * @param    x               Coordenada X do canto superior esquerdo.
  * @param    y               Coordenada Y do canto superior esquerdo.
  * @param    width           Largura do retângulo.
  * @param    height          Altura do retângulo.
  * @param    color           String com o código HEX da cor de preenchimento.
  * @param    border_color    String com o código HEX da cor da borda.
  * @param    border_width    String com o tamanho da borda, indicando também a unidade.
  *
  * @return   Uma instância de retângulo.
  * @warning  Em caso de erro na alocação de memória, o programa será encerrado.
  */
rectangle_t *rect_init(size_t id, double x, double y, double width, double height, const char *color, const char *border_color, const char *border_width);

/** @brief    Destrói um retângulo.
  *
  * @param    rectangle   Uma instância de retângulo.
  */
void rect_destroy(void *rectangle);

/** @brief    Define o X de um retângulo.
  *
  * @param    rect    Uma instância de retângulo.
  * @param    x       Coordenada X do canto superior esquerdo.
  */
void rect_set_x(rectangle_t *rect, double x);

/** @brief    Define o Y de um retângulo.
  *
  * @param    rect    Uma instância de retângulo.
  * @param    y       Coordenada Y do canto superior esquerdo.
  */
void rect_set_y(rectangle_t *rect, double y);

/** @brief    Define a largura de um retângulo.
  *
  * @param    rect    Uma instância de retângulo.
  * @param    width   Largura do retângulo.
  */
void rect_set_width(rectangle_t *rect, double width);

/** @brief    Define a altura de um retângulo.
  *
  * @param    rect    Uma instância de retângulo.
  * @param    height  Altura do retângulo.
  */
void rect_set_height(rectangle_t *rect, double height);

/** @brief    Define a cor de preenchimento de um retângulo.
  *
  * @param    rect    Uma instância de retângulo.
  * @param    color   String com o código HEX da cor de preenchimento.
  */
void rect_set_color(rectangle_t *rect, const char *color);

/** @brief    Define a cor da borda de um retângulo.
  *
  * @param    rect          Uma instância de retângulo.
  * @param    border_color  String com o código HEX da cor da borda.
  */
void rect_set_border_color(rectangle_t *rect, const char *border_color);

/** @brief    Define a largura da borda de um retângulo.
  *
  * @param    rect          Uma instância de retângulo.
  * @param    border_width  String com o tamanho da borda, indicando também a unidade.
  */
void rect_set_border_width(rectangle_t *rect, const char *border_width);

/** @brief    Retorna o ID de um retângulo.
  *
  * @param    rect    Uma instância de retângulo.
  *
  * @return   O ID de um retângulo.
  */
size_t rect_get_id(rectangle_t *rect);

/** @brief    Obtém o ponto de origem (canto inferior esquerdo) de um retângulo.
  *
  * @param    rect Pointer para um retângulo.
  *
  * @return   O ponto que representa a origem do retângulo.
  */
point_t *rect_get_origin(rectangle_t *rect);

/** @brief    Retorna o X de um retângulo.
  *
  * @param    rect    Uma instância de retângulo.
  *
  * @return   O X do canto superior esquerdo do retângulo.
  */
double rect_get_x(rectangle_t *rect);

/** @brief    Retorna o Y de um retângulo.
  *
  * @param    rect    Uma instância de retângulo.
  * @return   O Y do canto superior esquerdo do retângulo.
  */
double rect_get_y(rectangle_t *rect);

/** @brief    Retorna a largura de um retângulo.
  *
  * @param    rect    Uma instância de retângulo.
  *
  * @return   A largura do retângulo.
  */
double rect_get_width(rectangle_t *rect);

/** @brief    Retorna a altura de um retângulo.
  *
  * @param    rect    Uma instância de retângulo.
  *
  * @return   A altura do retângulo.
  */
double rect_get_height(rectangle_t *rect);

/** @brief    Retorna a cor de preenchimento de um retângulo.
  *
  * @param    rect    Uma instância de retângulo.
  *
  * @return   Uma string com a cor de preenchimento do retângulo.
  */
const char *rect_get_color(rectangle_t *rect);

/** @brief    Retorna a cor da borda de um retângulo.
  *
  * @param    rect    Uma instância de retângulo.
  *
  * @return   Uma string com a cor da borda do retângulo.
  */
const char *rect_get_border_color(rectangle_t *rect);

/** @brief    Retorna a largura da borda de um retângulo.
  *
  * @param    rect    Uma instância de retângulo.
  *
  * @return   String com o tamanho da borda.
  */
const char *rect_get_border_width(rectangle_t *rect);

#endif

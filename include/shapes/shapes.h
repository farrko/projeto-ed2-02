#ifndef SHAPES_H
#define SHAPES_H

#include <stdbool.h>

#include "circle.h"
#include "rectangle.h"
#include "text.h"
#include "line.h"
#include "path.h"

/*
 *  SHAPE
 *
 * O módulo de shape define a estrutura shape_t, um wrapper genérico capaz de
 * armazenar qualquer uma das formas geométricas suportadas pelo sistema: círculo, retângulo
 * ou texto. O tipo da forma armazenada é indicado pelo enumerador E_SHAPETYPE, que permite
 * identificar e recuperar a forma subjacente com segurança de tipos por meio das funções
 * shape_as_circle(), shape_as_rectangle() e shape_as_text().
 *
 * Essa abstração possibilita o armazenamento e a manipulação uniforme de formas heterogêneas
 * em estruturas de dados genéricas, como listas encadeadas.
 */

typedef struct shape_t shape_t;
typedef enum {
  CIRCLE,
  RECTANGLE,
  TEXT,
  LINE,
  PATH
} E_SHAPETYPE;

/** @brief    Inicialização de um shape.
  *
  * @param    type    O tipo da forma armazenada pelo shape.
  * @param    shape   Um pointer para a forma a ser armazenada.
  *
  * @return   Uma instância de shape.
  */
shape_t *shape_init(E_SHAPETYPE type, void *shape);

/** @brief    Destrói um shape e a sua forma associada.
  *
  * @param    shape   Uma instância de shape.
  */
void shape_destroy(void *shape);

/** @brief    Retorna o tipo da forma armazenada no shape.
  *
  * @param    shape   Uma instância de shape.
  *
  * @return   Um membro de E_SHAPETYPE que define o tipo do elemento associado.
  */
E_SHAPETYPE shape_get_type(shape_t *shape);

/** @brief    Retorna a forma associada ao shape como um círculo.
  *
  * @param    shape   Uma instância de shape.
  *
  * @return   Uma instância de círculo.
  */
circle_t *shape_as_circle(shape_t *shape);

/** @brief    Retorna a forma associada ao shape como um retângulo.
  *
  * @param    shape   Uma instância de shape.
  *
  * @return   Uma instância de retângulo.
  */
rectangle_t *shape_as_rectangle(shape_t *shape);

/** @brief    Retorna a forma associada ao shape como um texto.
  *
  * @param    shape   Uma instância de shape.
  *
  * @return   Uma instância de texto.
  */
text_t *shape_as_text(shape_t *shape);

/** @brief    Retorna a forma associada ao shape como uma linha.
  *
  * @param    shape   Uma instância de shape.
  *
  * @return   Uma instância de linha.
  */
line_t *shape_as_line(shape_t *shape);

path_t *shape_as_path(shape_t *shape);

#endif

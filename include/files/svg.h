#ifndef SVG_H
#define SVG_H

#include "shapes/circle.h"
#include "shapes/rectangle.h"
#include "shapes/text.h"
#include "shapes/line.h"
#include "datast/vector.h"

/*
 *  SVG
 *
 * O módulo SVG fornece uma interface para criação e escrita de arquivos no formato SVG
 * (Scalable Vector Graphics). A estrutura svg_t encapsula o arquivo de saída e expõe
 * operações de escrita para as formas geométricas suportadas: círculos, retângulos e textos.
 *
 * Um arquivo SVG deve ser inicializado com svg_init() e encerrado com svg_close(), que
 * finaliza a estrutura do documento e libera os recursos associados.
 */

typedef struct svg_t svg_t;

/** @brief    Inicializa um arquivo SVG para escrita.
  *
  * @param    path   Caminho do arquivo SVG a ser criado.
  *
  * @return   Uma instância de svg pronta para escrita.
  */
svg_t *svg_init(const char *path);

/** @brief    Fecha o arquivo SVG e libera os recursos associados.
  *
  * @param    svg    Uma instância de svg.
  */
void svg_close(svg_t *svg);

/** @brief    Obtém o caminho do arquivo associado ao SVG.
  *
  * @param    svg   Uma instância de svg.
  *
  * @return   Uma string contendo o path do arquivo SVG.
  */
const char *svg_get_path(svg_t *svg);

/** @brief    Escreve um círculo no arquivo SVG.
  *
  * @param    svg       Uma instância de svg.
  * @param    circle    A forma de círculo a ser escrita.
  */
void svg_write_circle(svg_t *svg, circle_t *circle);

/** @brief    Escreve um retângulo no arquivo SVG.
  *
  * @param    svg       Uma instância de svg.
  * @param    rect      A forma de retângulo a ser escrita.
  */
void svg_write_rectangle(svg_t *svg, rectangle_t *rect);

/** @brief    Escreve um texto no arquivo SVG.
  *
  * @param    svg       Uma instância de svg.
  * @param    text      A forma de texto a ser escrita.
  */
void svg_write_text(svg_t *svg, text_t *text);

/** @brief    Escreve uma linha no arquivo SVG.
  *
  * @param    svg       Uma instância de svg.
  * @param    line      A forma de linha a ser escrita.
  */
void svg_write_line(svg_t *svg, line_t *line);

/** @brief    Escreve todas as quadras de um vetor no arquivo SVG, cada uma como um
  *           retângulo.
  *
  * @param    svg     Uma instância de svg.
  * @param    blocks  Vetor de quadras (block_t) a serem escritas.
  */
void svg_write_blocks(svg_t *svg, vector_t *blocks);

/** @brief    Escreve um vetor de shapes inteiro no arquivo SVG.
  *
  * @param    svg    Uma instância de svg.
  * @param    v      Vetor de formas (shape_t) a serem escritas.
  */
void svg_write_vector_shape(svg_t *svg, vector_t *v);

#endif

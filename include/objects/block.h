#ifndef BLOCK_H
#define BLOCK_H

#include <stddef.h>

/*
 *  QUADRA
 *
 * O módulo de quadra (block) define a estrutura block_t, responsável por representar
 * uma quadra urbana. Cada quadra é identificada por um CEP e possui um ponto de ancoragem
 * (x, y) que define sua posição no espaço, além de dimensões geométricas (largura e altura).
 *
 * As propriedades de cor (cor de preenchimento e cor de borda) e espessura de borda destinam-se
 * à exibição visual da quadra. A estrutura pode ser criada por meio de block_init() e destruída
 * com block_destroy(). São disponibilizadas operações de leitura (get) para todos os campos e
 * de escrita (set) para os campos mutáveis.
 */

typedef struct block_t block_t;

/** @brief   Inicializa uma nova estrutura de quadra com os dados fornecidos.
 *
 * @param   cep           CEP identificador da quadra.
 * @param   x             Coordenada x do ponto de ancoragem da quadra.
 * @param   y             Coordenada y do ponto de ancoragem da quadra.
 * @param   width         Largura da quadra.
 * @param   height        Altura da quadra.
 * @param   color         Cor de preenchimento da quadra para exibição.
 * @param   border_color  Cor da borda da quadra para exibição.
 * @param   border_width  Espessura da borda da quadra para exibição.
 *
 * @return  Ponteiro para a nova estrutura block_t inicializada.
 */
block_t *block_init(const char *cep, double x, double y, double width, double height, const char *color, const char *border_color, const char *border_width);

/** @brief   Retorna o tamanho em bytes da estrutura block_t.
 *
 * @return  Tamanho em bytes de block_t.
 */
size_t block_sizeof();

/** @brief   Recupera o CEP identificador da quadra.
 *
 * @param   block  Ponteiro para a quadra.
 *
 * @return  Ponteiro para a string contendo o CEP.
 */
const char *block_get_cep(const block_t *block);

/** @brief   Recupera a coordenada x do ponto de ancoragem da quadra.
 *
 * @param   block  Ponteiro para a quadra.
 *
 * @return  Coordenada x da quadra.
 */
double block_get_x(const block_t *block);

/** @brief   Recupera a coordenada y do ponto de ancoragem da quadra.
 *
 * @param   block  Ponteiro para a quadra.
 *
 * @return  Coordenada y da quadra.
 */
double block_get_y(const block_t *block);

/** @brief   Recupera a largura da quadra.
 *
 * @param   block  Ponteiro para a quadra.
 *
 * @return  Largura da quadra.
 */
double block_get_width(const block_t *block);

/** @brief   Recupera a altura da quadra.
 *
 * @param   block  Ponteiro para a quadra.
 *
 * @return  Altura da quadra.
 */
double block_get_height(const block_t *block);

/** @brief   Recupera a cor de preenchimento da quadra.
 *
 * @param   block  Ponteiro para a quadra.
 *
 * @return  Ponteiro para a string contendo a cor de preenchimento.
 */
const char *block_get_color(const block_t *block);

/** @brief   Recupera a cor da borda da quadra.
 *
 * @param   block  Ponteiro para a quadra.
 *
 * @return  Ponteiro para a string contendo a cor da borda.
 */
const char *block_get_border_color(const block_t *block);

/** @brief   Recupera a espessura da borda da quadra.
 *
 * @param   block  Ponteiro para a quadra.
 *
 * @return  Espessura da borda da quadra.
 */
const char *block_get_border_width(const block_t *block);

/** @brief   Define a coordenada x do ponto de ancoragem da quadra.
 *
 * @param   block  Ponteiro para a quadra.
 * @param   x      Nova coordenada x a ser atribuída.
 */
void block_set_x(block_t *block, double x);

/** @brief   Define a coordenada y do ponto de ancoragem da quadra.
 *
 * @param   block  Ponteiro para a quadra.
 * @param   y      Nova coordenada y a ser atribuída.
 */
void block_set_y(block_t *block, double y);

/** @brief   Define a largura da quadra.
 *
 * @param   block  Ponteiro para a quadra.
 * @param   w      Nova largura a ser atribuída.
 */
void block_set_width(block_t *block, double w);

/** @brief   Define a altura da quadra.
 *
 * @param   block  Ponteiro para a quadra.
 * @param   h      Nova altura a ser atribuída.
 */
void block_set_height(block_t *block, double h);

/** @brief   Define a cor de preenchimento da quadra.
 *
 * @param   block  Ponteiro para a quadra.
 * @param   color  Nova string de cor de preenchimento a ser atribuída.
 */
void block_set_color(block_t *block, const char *color);

/** @brief   Define a cor da borda da quadra.
 *
 * @param   block         Ponteiro para a quadra.
 * @param   border_color  Nova string de cor de borda a ser atribuída.
 */
void block_set_border_color(block_t *block, const char *border_color);

/** @brief   Define a espessura da borda da quadra.
 *
 * @param   block         Ponteiro para a quadra.
 * @param   border_width  Nova espessura de borda a ser atribuída.
 */
void block_set_border_width(block_t *block, const char *border_width);

/** @brief   Destrói a estrutura de quadra, liberando a memória alocada.
 *
 * @param   block  Ponteiro para a quadra a ser destruída.
 */
void block_destroy(block_t *block);

#endif

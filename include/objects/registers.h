#ifndef REGISTERS_H
#define REGISTERS_H

#include "shapes/point.h"

/*
 *  REGISTRADORES
 *
 * O módulo de registradores (registers) define a estrutura registers_t, responsável por
 * armazenar referências geográficas obtidas durante o processamento de um arquivo .qry. Cada
 * referência é um point_t representando a posição absoluta de um endereço, armazenado em um
 * registrador identificado por um índice entre 0 e REGISTERS_MAX_INDEX.
 *
 * As referências armazenadas em um registrador podem ser reutilizadas por outras consultas
 * do mesmo arquivo .qry, sendo definidas pelo comando @o? e consultadas posteriormente
 * conforme a necessidade de cada operação.
 *
 * A estrutura assume a posse de todos os pontos armazenados em seus registradores: ao
 * substituir o valor de um registrador ou ao destruir a estrutura, os pontos previamente
 * armazenados são liberados automaticamente.
 *
 * A estrutura pode ser criada por meio de registers_init() e destruída com
 * registers_destroy(). A leitura (get) e escrita (set) de cada registrador são
 * disponibilizadas individualmente.
 */

#define REGISTERS_MAX_INDEX 10

typedef struct registers_t registers_t;

/** @brief    Inicializa uma estrutura de registradores, todos inicialmente vazios.
  *
  * @return   Uma instância de registers_t pronta para uso.
  * @warning  Em caso de erro na alocação de memória, o programa será encerrado.
  */
registers_t *registers_init(void);

/** @brief    Destrói uma estrutura de registradores, liberando também todos os pontos
  *           armazenados em seus registradores.
  *
  * @param    registers  Pointer para a estrutura de registradores a ser destruída.
  */
void registers_destroy(registers_t *registers);

/** @brief    Define o ponto armazenado em um registrador, assumindo a posse do ponto
  *           fornecido.
  *
  * @param    registers  Pointer para a estrutura de registradores.
  * @param    index      Índice do registrador, entre 0 e REGISTERS_MAX_INDEX.
  * @param    point      Pointer para o ponto a ser armazenado.
  *
  * @note     Caso o registrador já possua um ponto armazenado, este é destruído antes da
  *           substituição.
  * @warning  Caso @p index esteja fora do intervalo válido, nenhuma operação é realizada e o
  *           ponto fornecido não é armazenado nem liberado, permanecendo sob posse do
  *           chamador.
  */
void registers_set(registers_t *registers, int index, point_t *point);

/** @brief    Obtém o ponto armazenado em um registrador.
  *
  * @param    registers  Pointer para a estrutura de registradores.
  * @param    index      Índice do registrador, entre 0 e REGISTERS_MAX_INDEX.
  *
  * @return   Pointer para o ponto armazenado no registrador, ou @c NULL caso o registrador
  *           esteja vazio ou @p index esteja fora do intervalo válido.
  */
point_t *registers_get(registers_t *registers, int index);

#endif

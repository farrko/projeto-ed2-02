#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stddef.h>
#include "nodes.h"

/**
 * @file   linkedlist.h
 * @brief  Implementação de uma lista duplamente ligada genérica, construída sobre node_t.
 *
 * O módulo linkedlist implementa uma lista duplamente ligada (doubly linked list) cujos
 * elementos são instâncias de node_t (ver nodes.h), cada uma já pronta com o dado
 * arbitrário e a função de destruição desejados no momento em que é inserida na lista.
 *
 * A lista é responsável apenas por gerenciar a estrutura de encadeamento (os pointers lpt
 * e rpt de cada nó, bem como head, tail e length); a posse do dado interno de cada nó
 * (value) permanece sob responsabilidade do próprio node_t, conforme definido em sua
 * inicialização.
 *
 * Operações de remoção estão disponíveis em duas variantes: as prefixadas com "pop"
 * (llist_popat_*) desconectam o nó da lista e o devolvem ao chamador, que passa a ser
 * responsável por destruí-lo; llist_destroyat_index() remove e já destrói o nó
 * correspondente, liberando também seu dado interno (caso uma função de destruição tenha
 * sido associada a ele).
 */

typedef struct llist_t llist_t;

/** @brief    Inicializa uma lista duplamente ligada vazia.
  *
  * @return   Uma instância de llist_t pronta para uso.
  * @warning  Em caso de erro na alocação de memória, o programa será encerrado.
  */
llist_t *llist_init();

/** @brief    Destrói uma lista, liberando a memória de todos os seus nós.
  *
  * @param    llist  Pointer para a lista a ser destruída.
  *
  * @note     Para cada nó da lista, o dado interno é liberado por meio de node_destroy(),
  *           caso uma função de destruição tenha sido associada ao nó em node_init().
  */
void llist_destroy(llist_t *llist);

/** @brief    Obtém o primeiro nó da lista.
  *
  * @param    llist  Pointer para a lista.
  *
  * @return   Pointer para o primeiro nó, ou @c NULL caso a lista esteja vazia.
  */
node_t *llist_get_head(llist_t *llist);

/** @brief    Obtém o último nó da lista.
  *
  * @param    llist  Pointer para a lista.
  *
  * @return   Pointer para o último nó, ou @c NULL caso a lista esteja vazia.
  */
node_t *llist_get_tail(llist_t *llist);

/** @brief    Obtém a quantidade de nós presentes na lista.
  *
  * @param    llist  Pointer para a lista.
  *
  * @return   O número de nós atualmente encadeados na lista.
  */
size_t llist_get_length(llist_t *llist);

/** @brief    Acessa o nó na posição especificada da lista.
  *
  * @param    llist  Pointer para a lista.
  * @param    index  Índice do nó desejado, a partir de @c 0 (head).
  *
  * @return   Pointer para o nó na posição @p index, ou @c NULL caso o índice esteja fora
  *           dos limites da lista.
  * @warning  Esta operação percorre a lista linearmente a partir do head até @p index.
  */
node_t *llist_getat_index(llist_t *llist, size_t index);

/** @brief    Remove o nó na posição especificada, sem destruí-lo.
  *
  * @param    llist  Pointer para a lista.
  * @param    index  Índice do nó a ser removido, a partir de @c 0 (head).
  *
  * @return   Pointer para o nó removido, já desconectado da lista, ou @c NULL caso o índice
  *           esteja fora dos limites.
  * @warning  O nó retornado não é destruído: o dado interno e a estrutura do nó permanecem
  *           de responsabilidade do chamador, que deve liberá-los (ver node_destroy()) caso
  *           não pretenda reinseri-lo em outra lista.
  */
node_t *llist_popat_index(llist_t *llist, size_t index);

/** @brief    Remove o primeiro nó da lista, sem destruí-lo.
  *
  * @param    llist  Pointer para a lista.
  *
  * @return   Pointer para o nó removido, ou @c NULL caso a lista esteja vazia.
  * @warning  Ver aviso de llist_popat_index() quanto à responsabilidade sobre o nó
  *           retornado.
  */
node_t *llist_popat_start(llist_t *llist);

/** @brief    Remove o último nó da lista, sem destruí-lo.
  *
  * @param    llist  Pointer para a lista.
  *
  * @return   Pointer para o nó removido, ou @c NULL caso a lista esteja vazia.
  * @warning  Ver aviso de llist_popat_index() quanto à responsabilidade sobre o nó
  *           retornado.
  */
node_t *llist_popat_end(llist_t *llist);

/** @brief    Insere um nó na posição especificada da lista.
  *
  * @param    llist  Pointer para a lista.
  * @param    node   Pointer para o nó a ser inserido, já inicializado (ver node_init()).
  * @param    index  Posição na qual o nó será inserido, a partir de @c 0 (head).
  *
  * @note     Caso @p index seja maior que o comprimento atual da lista, nenhuma operação é
  *           realizada.
  * @warning  Os pointers de vizinhança (lpt e rpt) de @p node são sobrescritos por esta
  *           operação, de forma a refletir sua nova posição na lista.
  */
void llist_insertat_index(llist_t *llist, node_t *node, size_t index);

/** @brief    Insere um nó no início da lista.
  *
  * @param    llist  Pointer para a lista.
  * @param    node   Pointer para o nó a ser inserido, já inicializado (ver node_init()).
  */
void llist_insertat_start(llist_t *llist, node_t *node);

/** @brief    Insere um nó ao final da lista.
  *
  * @param    llist  Pointer para a lista.
  * @param    node   Pointer para o nó a ser inserido, já inicializado (ver node_init()).
  */
void llist_insertat_end(llist_t *llist, node_t *node);

/** @brief    Remove e destrói o nó na posição especificada da lista.
  *
  * @param    llist  Pointer para a lista.
  * @param    index  Índice do nó a ser removido e destruído, a partir de @c 0 (head).
  *
  * @note     Caso o índice esteja fora dos limites da lista, nenhuma operação é realizada.
  * @note     Diferentemente das operações llist_popat_*, esta função já libera a memória do
  *           nó removido (e de seu dado interno, caso aplicável) por meio de
  *           node_destroy().
  */
void llist_destroyat_index(llist_t *llist, size_t index);

#endif

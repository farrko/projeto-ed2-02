#ifndef NODES_H
#define NODES_H

/**
 * @file   nodes.h
 * @brief  Implementação de um nó duplamente ligado, genérico, para uso em listas encadeadas.
 *
 * O módulo nodes define a estrutura node_t, um nó capaz de armazenar um dado arbitrário
 * (void*) e manter pointers para seus vizinhos esquerdo (lpt) e direito (rpt), permitindo
 * a construção de listas duplamente ligadas (ver linkedlist.h).
 *
 * A estrutura pode assumir ownership opcional sobre o dado armazenado: node_init() recebe
 * uma função de destruição, invocada automaticamente por node_destroy() e por
 * node_set_value() ao substituir um valor já existente. Caso o nó não deva gerenciar o
 * ciclo de vida do dado, esta função pode ser @c NULL.
 */

typedef struct node_t node_t;

/** @brief    Inicializa um nó com o dado e a função de destruição fornecidos.
  *
  * @param    value        Pointer para o dado arbitrário a ser armazenado no nó.
  * @param    destructor   Função utilizada para liberar @p value quando o nó for destruído
  *                         ou tiver seu valor substituído (ver node_destroy() e
  *                         node_set_value()). Pode ser @c NULL caso o nó não deva assumir
  *                         ownership sobre o dado.
  *
  * @return   Uma instância de node_t pronta para uso, com os vizinhos (lpt e rpt)
  *           inicialmente nulos.
  * @warning  Em caso de erro na alocação de memória, o programa será encerrado.
  */
node_t *node_init(void *value, void (*destructor)(void*));

/** @brief    Define o dado armazenado no nó, substituindo o valor anterior.
  *
  * @param    node   Pointer para o nó.
  * @param    value  Pointer para o novo dado a ser armazenado.
  *
  * @note     Caso o nó já possua um valor não nulo e uma função de destruição tenha sido
  *           fornecida em node_init(), o valor anterior é liberado por meio dela antes da
  *           substituição.
  */
void node_set_value(node_t *node, void *value);

/** @brief    Define a função de destruição a ser utilizada para o dado do nó.
  *
  * @param    node        Pointer para o nó.
  * @param    destructor  Nova função de destruição a ser associada ao nó.
  */
void node_set_destructor(node_t *node, void (*destructor)(void *));

/** @brief    Define o vizinho à esquerda (predecessor) do nó.
  *
  * @param    node  Pointer para o nó.
  * @param    lpt   Pointer para o nó a ser definido como predecessor.
  */
void node_set_lpt(node_t *node, node_t *lpt);

/** @brief    Define o vizinho à direita (sucessor) do nó.
  *
  * @param    node  Pointer para o nó.
  * @param    rpt   Pointer para o nó a ser definido como sucessor.
  */
void node_set_rpt(node_t *node, node_t *rpt);

/** @brief    Obtém o dado armazenado no nó.
  *
  * @param    node  Pointer para o nó.
  *
  * @return   Pointer para o dado armazenado.
  */
void *node_get_value(node_t *node);

/** @brief    Obtém o vizinho à esquerda (predecessor) do nó.
  *
  * @param    node  Pointer para o nó.
  *
  * @return   Pointer para o nó predecessor, ou @c NULL caso não exista.
  */
node_t *node_get_lpt(node_t *node);

/** @brief    Obtém o vizinho à direita (sucessor) do nó.
  *
  * @param    node  Pointer para o nó.
  *
  * @return   Pointer para o nó sucessor, ou @c NULL caso não exista.
  */
node_t *node_get_rpt(node_t *node);

/** @brief    Destrói um nó, liberando a memória de sua estrutura.
  *
  * @param    node  Pointer para o nó a ser destruído.
  *
  * @note     Caso o nó possua um valor não nulo e uma função de destruição tenha sido
  *           fornecida em node_init(), ela é invocada sobre o valor antes da liberação do
  *           nó.
  * @warning  Esta função não desconecta o nó de seus vizinhos (lpt e rpt); caso o nó esteja
  *           inserido em uma lista, prefira removê-lo por meio das operações da própria
  *           lista (ver linkedlist.h) para manter a consistência dos pointers vizinhos.
  */
void node_destroy(node_t *node);

#endif

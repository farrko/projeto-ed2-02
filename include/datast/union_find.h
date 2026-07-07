#ifndef UNION_FIND_H
#define UNION_FIND_H

#include <stddef.h>
#include <stdbool.h>

/**
 * @file   union_find.h
 * @brief  Implementação de uma estrutura de conjuntos disjuntos (union-find / DSU).
 *
 * O módulo union_find implementa uma estrutura de conjuntos disjuntos sobre um universo
 * fixo de @c size elementos, identificados pelos índices @c 0 a <tt>size - 1</tt>. Cada
 * elemento inicia em seu próprio conjunto (singleton) e conjuntos podem ser progressivamente
 * unidos por meio de uf_unite().
 *
 * A implementação utiliza compressão de caminho (path compression) em uf_find(): ao localizar
 * a raiz de um elemento, todos os nós percorridos no caminho até ela são reapontados
 * diretamente para a raiz, achatando a árvore e acelerando buscas futuras.
 *
 * Esta estrutura é utilizada, por exemplo, por graph_kruskal() (ver graphs.h) para detectar
 * ciclos durante a construção da árvore geradora mínima, unindo os componentes dos nós à
 * medida que arestas são aceitas.
 */

typedef struct ufind_t ufind_t;

/** @brief    Inicializa uma estrutura de union-find com @p size elementos, cada um em seu
  *           próprio conjunto.
  *
  * @param    size  Quantidade de elementos do universo, identificados pelos índices
  *                 @c 0 a <tt>size - 1</tt>.
  *
  * @return   Uma instância de ufind_t pronta para uso.
  * @warning  Em caso de erro na alocação de memória, o programa será encerrado.
  */
ufind_t *uf_init(size_t size);

/** @brief    Une os conjuntos aos quais pertencem os elementos @p p e @p q.
  *
  * @param    uf  Pointer para a estrutura de union-find.
  * @param    p   Índice do primeiro elemento.
  * @param    q   Índice do segundo elemento.
  *
  * @note     Caso @p p e @p q já pertençam ao mesmo conjunto, nenhuma operação é realizada.
  */
void uf_unite(ufind_t *uf, size_t p, size_t q);

/** @brief    Encontra o identificador da raiz do conjunto ao qual pertence o elemento @p p.
  *
  * @param    uf  Pointer para a estrutura de union-find.
  * @param    p   Índice do elemento cujo conjunto se deseja identificar.
  *
  * @return   O índice da raiz do conjunto ao qual @p p pertence. Dois elementos pertencem
  *           ao mesmo conjunto se, e somente se, uf_find() retornar o mesmo valor para
  *           ambos.
  * @note     Aplica compressão de caminho: todos os elementos percorridos até a raiz durante
  *           a busca passam a apontar diretamente para ela, otimizando chamadas futuras.
  */
size_t uf_find(ufind_t *uf, size_t p);

/** @brief    Destrói uma estrutura de union-find, liberando a memória alocada.
  *
  * @param    uf  Pointer para a estrutura de union-find a ser destruída.
  */
void uf_destroy(ufind_t *uf);

#endif

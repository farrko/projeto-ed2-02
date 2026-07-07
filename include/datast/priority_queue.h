#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

/**
 * @file   priority_queue.h
 * @brief  Implementação de uma fila de prioridades (min-heap) genérica.
 *
 * O módulo priority_queue implementa uma fila de prioridades por meio de um heap binário
 * de mínimo (min-heap), armazenado internamente sobre um vector_t. Cada elemento inserido
 * é associado a uma prioridade numérica (double); o elemento de menor prioridade é sempre
 * o próximo a ser removido por pq_dequeue(), independentemente da ordem de inserção.
 *
 * A estrutura não assume ownership sobre os dados enfileirados: apenas o pointer fornecido
 * em pq_enqueue() é armazenado, e cabe ao chamador gerenciar o ciclo de vida da informação
 * apontada, inclusive após a remoção via pq_dequeue().
 *
 * Este módulo é utilizado, por exemplo, por graph_dijkstra() (ver graphs.h) para selecionar,
 * a cada passo, o nó de menor custo acumulado ainda não visitado.
 */

typedef struct priqueue_t priqueue_t;

/** @brief    Inicializa uma fila de prioridades vazia.
  *
  * @return   Uma instância de priqueue_t pronta para uso.
  * @warning  Em caso de erro na alocação de memória, o programa será encerrado.
  */
priqueue_t *pq_init();

/** @brief    Destrói uma fila de prioridades, liberando a memória de sua estrutura interna.
  *
  * @param    pq  Pointer para a fila de prioridades a ser destruída.
  *
  * @warning  Os dados apontados pelos elementos ainda presentes na fila não são liberados
  *           por esta função; a estrutura não possui ownership sobre eles.
  */
void pq_destroy(priqueue_t *pq);

/** @brief    Insere um elemento na fila de prioridades.
  *
  * @param    pq        Pointer para a fila de prioridades.
  * @param    data      Pointer para o dado arbitrário a ser enfileirado.
  * @param    priority  Valor de prioridade associado ao dado; quanto menor, mais cedo o
  *                      elemento será removido por pq_dequeue().
  *
  * @warning  Em caso de erro na realocação do buffer interno, o programa será encerrado.
  */
void pq_enqueue(priqueue_t *pq, void *data, double priority);

/** @brief    Remove e retorna o elemento de menor prioridade da fila.
  *
  * @param    pq  Pointer para a fila de prioridades.
  *
  * @return   Pointer para o dado associado ao elemento de menor prioridade, ou @c NULL
  *           caso a fila esteja vazia.
  * @note     Em caso de empate de prioridade entre dois ou mais elementos, não há garantia
  *           sobre qual deles será removido primeiro.
  */
void *pq_dequeue(priqueue_t *pq);

#endif

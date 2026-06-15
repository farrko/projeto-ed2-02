#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
#include <stdbool.h>

/**
 * @file   vector.h
 * @brief  Implementação de um vetor dinâmico genérico análogo ao std::vector do C++.
 *
 * O módulo vector implementa um array dinâmico de elementos homogêneos com tamanho
 * fixo em bytes. A estrutura vector_t gerencia um buffer interno redimensionável,
 * expondo operações de acesso, inserção e remoção típicas de um contêiner sequencial.
 *
 * Esta estrutura trabalha com a cópia, byte a byte, dos dados inseridos.
 */

typedef struct vector_t vector_t;

/** @brief    Inicializa um vetor dinâmico.
  *
  * @param    data_size  Tamanho em bytes de cada elemento a ser armazenado.
  *
  * @return   Uma instância de vector_t pronta para uso.
  * @warning  Em caso de erro na alocação de memória, o programa será encerrado.
  */
vector_t *vec_init(size_t data_size);

/** @brief    Destrói um vetor e libera toda a memória associada ao seu buffer interno.
  *
  * @param    v  Pointer para o vetor a ser destruído.
  */
void vec_destroy(vector_t *v);

/** @brief    Obtém a quantidade de elementos presentes no vetor.
  *
  * @param    v  Pointer para o vetor.
  *
  * @return   O número de elementos atualmente armazenados.
  */
size_t vec_get_size(vector_t *v);

/** @brief    Verifica se o vetor está vazio.
  *
  * @param    v  Pointer para o vetor.
  *
  * @return   @c true se o vetor não contiver elementos; @c false caso contrário.
  */
bool vec_is_empty(vector_t *v);

/** @brief    Acessa o elemento no índice especificado.
  *
  * @param    v      Pointer para o vetor.
  * @param    index  Índice do elemento desejado.
  *
  * @return   Pointer para o elemento na posição @p index, ou @c NULL se o índice
  *           estiver fora dos limites do vetor.
  */
void *vec_at(vector_t *v, size_t index);

/** @brief    Acessa o primeiro elemento do vetor.
  *
  * @param    v  Pointer para o vetor.
  *
  * @return   Pointer para o primeiro elemento, ou @c NULL se o vetor estiver vazio.
  */
void *vec_front(vector_t *v);

/** @brief    Acessa o último elemento do vetor.
  *
  * @param    v  Pointer para o vetor.
  *
  * @return   Pointer para o último elemento, ou @c NULL se o vetor estiver vazio.
  */
void *vec_back(vector_t *v);

/** @brief    Insere um elemento na posição especificada do vetor.
  *
  * @param    v      Pointer para o vetor.
  * @param    data   Pointer para os dados a serem copiados para o vetor.
  * @param    index  Posição na qual o elemento será inserido.
  *
  * @return   @c true se a inserção for bem-sucedida; @c false se @p index for maior
  *           do que o tamanho atual do vetor. Automaticamente chama vec_push_back caso
  *           o index for igual o tamanho do vetor.
  * @warning  Como em uma operação de vetor padrão, a inserção sobrescreve qualquer dado
  *           que anteriormente estava na região.
  */
bool vec_insert_at(vector_t *v, void *data, size_t index);

/** @brief    Insere um elemento ao final do vetor.
  *
  * @param    v     Pointer para o vetor.
  * @param    data  Pointer para os dados a serem copiados para o vetor.
  *
  * @warning  Em caso de erro na realocação do buffer interno, o programa será encerrado.
  */
void vec_push_back(vector_t *v, void *data);

/** @brief    Remove o último elemento do vetor.
  *
  * @param    v  Pointer para o vetor.
  *
  * @warning  Não realiza nenhuma operação caso o vetor esteja vazio.
  */
void vec_pop_back(vector_t *v);

/** @brief    Clona um vetor, criando uma cópia superficial independente.
  *
  * @param    v         Pointer para o vetor a ser clonado.
  *
  * @return   Pointer para um novo vetor com os mesmos elementos e tamanho
  *           do vetor original. O vetor retornado é independente do original
  *           e deve ser destruído pelo chamador com @p vec_destroy.
  *
  * @warning  A cópia é superficial: elementos que sejam ponteiros continuarão
  *           apontando para os mesmos endereços de memória do vetor original.
  *           Modificações nos dados apontados afetarão ambos os vetores.
  */
vector_t *vec_clone(vector_t *v);

#endif

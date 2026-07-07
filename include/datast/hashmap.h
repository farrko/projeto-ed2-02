#ifndef HASHMAP_H
#define HASHMAP_H

#include <stddef.h>
#include <stdint.h>

/**
 * @file   hashmap.h
 * @brief  Implementação de uma tabela hash genérica com chaves em string.
 *
 * O módulo hashmap implementa uma tabela hash de capacidade fixa, definida no momento da
 * inicialização, com resolução de colisões por encadeamento separado (cada posição da
 * tabela é uma lista ligada de pares chave-valor). As chaves são strings, internamente
 * convertidas para uma chave numérica (djb2) e distribuídas entre as posições da tabela por
 * meio de uma função de hash adicional (baseada em FNV).
 *
 * A estrutura assume ownership opcional sobre os valores armazenados: hm_set() recebe uma
 * função de destruição por entrada, invocada automaticamente quando a entrada é liberada em
 * hm_destroy() (ou sobrescrita, dependendo da implementação de armazenamento subjacente).
 * Caso o valor não deva ser gerenciado pela tabela, esta função pode ser @c NULL.
 */

typedef struct hashmap_t hashmap_t;

/** @brief    Inicializa uma tabela hash vazia com a capacidade especificada.
  *
  * @param    capacity  Número de posições (buckets) da tabela.
  *
  * @return   Uma instância de hashmap_t pronta para uso.
  * @warning  Em caso de erro na alocação de memória, o programa será encerrado.
  */
hashmap_t *hm_init(size_t capacity);

/** @brief    Destrói uma tabela hash, liberando toda a memória de sua estrutura interna.
  *
  * @param    hm  Pointer para a tabela hash a ser destruída.
  *
  * @note     Para cada entrada armazenada, caso uma função de destruição tenha sido
  *           fornecida em hm_set(), ela é invocada sobre o valor correspondente antes da
  *           liberação da entrada.
  */
void hm_destroy(hashmap_t *hm);

/** @brief    Obtém o valor associado a uma chave.
  *
  * @param    hm   Pointer para a tabela hash.
  * @param    key  Chave a ser buscada.
  *
  * @return   Pointer para o valor associado a @p key, ou @c NULL caso a chave não esteja
  *           presente na tabela.
  */
void *hm_get(hashmap_t *hm, const char *key);

/** @brief    Insere ou atualiza o valor associado a uma chave.
  *
  * @param    hm          Pointer para a tabela hash.
  * @param    key         Chave a ser inserida ou atualizada.
  * @param    value       Pointer para o valor a ser associado à chave.
  * @param    destructor  Função utilizada para liberar @p value quando a entrada for
  *                        destruída (ver hm_destroy()). Pode ser @c NULL caso a tabela não
  *                        deva assumir ownership sobre o valor.
  *
  * @note     A chave é armazenada por valor (copiada internamente); o chamador permanece
  *           responsável pela memória apontada por @p key.
  * @warning  Em caso de erro na alocação de memória, o programa será encerrado.
  */
void hm_set(hashmap_t *hm, const char *key, void *value, void (*destructor)(void *));

#endif

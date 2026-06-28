#include <stdlib.h>
#include <stdio.h>

#include "objects/registers.h"

struct registers_t {
  point_t *slots[REGISTERS_MAX_INDEX + 1];
};

static int is_valid_index(int index) {
  return index >= 0 && index <= REGISTERS_MAX_INDEX;
}

registers_t *registers_init(void) {
  registers_t *registers = malloc(sizeof(registers_t));
  if (registers == NULL) {
    printf("Erro na alocação de memória.\n");
    exit(1);
  }

  for (int i = 0; i <= REGISTERS_MAX_INDEX; i++) registers->slots[i] = NULL;

  return registers;
}

void registers_destroy(registers_t *registers) {
  for (int i = 0; i <= REGISTERS_MAX_INDEX; i++) {
    if (registers->slots[i] != NULL) point_destroy(registers->slots[i]);
  }

  free(registers);
}

void registers_set(registers_t *registers, int index, point_t *point) {
  if (!is_valid_index(index)) return;

  if (registers->slots[index] != NULL) point_destroy(registers->slots[index]);

  registers->slots[index] = point;
}

point_t *registers_get(registers_t *registers, int index) {
  if (!is_valid_index(index)) return NULL;

  return registers->slots[index];
}

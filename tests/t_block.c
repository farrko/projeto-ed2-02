#include "unity/unity.h"
#include "objects/block.h"

void setUp(void) {}
void tearDown(void) {}

void test_block_sizeof_deve_ser_maior_que_zero(void) {
  TEST_ASSERT_GREATER_THAN(0, block_sizeof());
}

void test_block_init_retorna_ponteiro_nao_nulo(void) {
  block_t *b = block_init("86010-000", 10.0, 20.0, 100.0, 200.0, "red", "black", "2px");
  TEST_ASSERT_NOT_NULL(b);
  block_destroy(b);
}

void test_block_getters_retornam_valores_iniciais_corretos(void) {
  block_t *b = block_init("86010-000", 10.5, 20.5, 100.0, 200.0, "red", "black", "2px");

  TEST_ASSERT_EQUAL_STRING("86010-000", block_get_cep(b));

  TEST_ASSERT_DOUBLE_WITHIN(0.001, 10.5, block_get_x(b));
  TEST_ASSERT_DOUBLE_WITHIN(0.001, 20.5, block_get_y(b));

  block_destroy(b);
}

void test_block_setters_atualizam_dimensoes_e_coordenadas(void) {
  block_t *b = block_init("0000", 0.0, 0.0, 10.0, 10.0, "white", "black", "1px");

  block_set_y(b, 55.5);
  block_set_width(b, 150.0);
  block_set_height(b, 300.0);

  TEST_ASSERT_DOUBLE_WITHIN(0.001, 55.5, block_get_y(b));

  block_destroy(b);
}

int main(void) {
  UNITY_BEGIN();

  // sizeof
  RUN_TEST(test_block_sizeof_deve_ser_maior_que_zero);

  // criação
  RUN_TEST(test_block_init_retorna_ponteiro_nao_nulo);

  // getters
  RUN_TEST(test_block_getters_retornam_valores_iniciais_corretos);

  // setters
  RUN_TEST(test_block_setters_atualizam_dimensoes_e_coordenadas);

  return UNITY_END();
}

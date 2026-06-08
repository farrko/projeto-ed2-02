#include "unity/unity.h"
#include "datast/priority_queue.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
  char string[25];
  int number;
  double frac;
} test_t;

void setUp() {}
void tearDown() {}

/* ── pq_init ───────────────────────────────────────────────────────────────── */
void test_pq_init_retorna_nao_nulo() {
  priqueue_t *pq = pq_init();
  TEST_ASSERT_NOT_NULL(pq);
  pq_destroy(pq);
}

/* ── pq_enqueue / pq_dequeue ───────────────────────────────────────────────── */

void test_pq_enqueue_em_ordem() {
  priqueue_t *pq = pq_init();

  int a = 10;
  int b = 20;
  int c = 30;

  pq_enqueue(pq, &a, 1);
  pq_enqueue(pq, &b, 2);
  pq_enqueue(pq, &c, 3);

  TEST_ASSERT_EQUAL_INT(a, *(int *)pq_dequeue(pq));
  TEST_ASSERT_EQUAL_INT(b, *(int *)pq_dequeue(pq));
  TEST_ASSERT_EQUAL_INT(c, *(int *)pq_dequeue(pq));
}

void test_pq_enqueue_fora_de_ordem() {
  priqueue_t *pq = pq_init();

  int a = 10;
  int b = 20;
  int c = 30;

  pq_enqueue(pq, &a, 40);
  pq_enqueue(pq, &b, 15);
  pq_enqueue(pq, &c, 25);

  TEST_ASSERT_EQUAL_INT(b, *(int *)pq_dequeue(pq));
  TEST_ASSERT_EQUAL_INT(c, *(int *)pq_dequeue(pq));
  TEST_ASSERT_EQUAL_INT(a, *(int *)pq_dequeue(pq));
}

void test_pq_enqueue_structs() {
  priqueue_t *pq = pq_init();

  test_t *a = malloc(sizeof(test_t));
  strcpy(a->string, "abcde");
  a->number = 10;
  a->frac = 0.75;

  test_t *b = malloc(sizeof(test_t));
  strcpy(a->string, "fghij");
  a->number = 5;
  a->frac = 1.75;

  test_t *c = malloc(sizeof(test_t));
  strcpy(a->string, "klmno");
  a->number = 50;
  a->frac = -0.75;

  pq_enqueue(pq, a, 0.7);
  pq_enqueue(pq, b, 0.8);
  pq_enqueue(pq, c, 0.75);

  test_t *r1 = pq_dequeue(pq);
  test_t *r2 = pq_dequeue(pq);
  test_t *r3 = pq_dequeue(pq);

  TEST_ASSERT_EQUAL_PTR(a, r1);
  TEST_ASSERT_EQUAL_PTR(c, r2);
  TEST_ASSERT_EQUAL_PTR(b, r3);
}

void test_pq_dequeue_retorna_null_quando_vazio() {
  priqueue_t *pq = pq_init();

  TEST_ASSERT_NULL(pq_dequeue(pq));
}

/* ── main ──────────────────────────────────────────────────────────────────── */

int main() {
  UNITY_BEGIN();

  RUN_TEST(test_pq_init_retorna_nao_nulo);

  RUN_TEST(test_pq_enqueue_em_ordem);
  RUN_TEST(test_pq_enqueue_fora_de_ordem);
  RUN_TEST(test_pq_enqueue_structs);

  RUN_TEST(test_pq_dequeue_retorna_null_quando_vazio);

  return UNITY_END();
}

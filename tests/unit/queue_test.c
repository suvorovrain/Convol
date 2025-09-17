#include "../../src/utils/include/queue.h"
// clang-format off
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <cmocka.h>
// clang-format on

static void test_fifo(void **state)
{
    (void)state;
    b_queue *q = bq_init(2);
    assert_non_null(q);

    int *a = malloc(sizeof(int));
    int *b = malloc(sizeof(int));
    *a = 42;
    *b = 7;
    bq_enqueue(q, a);
    bq_enqueue(q, b);

    int *r1 = bq_dequeue(q);
    int *r2 = bq_dequeue(q);
    assert_int_equal(*r1, 42);
    assert_int_equal(*r2, 7);

    free(r1);
    free(r2);
    bq_destroy(q);
}

static void test_in_out_task(void **state)
{
    (void)state;
    in_task *in = create_in_task(1, NULL, strdup("f.bmp"));
    assert_non_null(in);
    free_in_task(in);

    out_task *out = create_out_task(2, NULL, strdup("o.bmp"));
    assert_non_null(out);
    free_out_task(out);
}

int queue_tests(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_fifo),
        cmocka_unit_test(test_in_out_task),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}

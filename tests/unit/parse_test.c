#include "../../src/utils/include/parse.h"
// clang-format off
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <cmocka.h>
// clang-format on
static void test_effect(void **state)
{
    (void)state;
    assert_true(parse_effect("blur") == BLUR);
    assert_true(parse_effect("motion_blur") == MOTION_BLUR);
    assert_true(parse_effect("find_edges") == FIND_EDGES);
    assert_true(parse_effect("emboss") == EMBOSS);
    assert_true(parse_effect("drain") == (enum effect) - 1);
}

static void test_strength(void **state)
{
    (void)state;
    assert_true(parse_effect_strength("1") == SMALL);
    assert_true(parse_effect_strength("2") == MEDIUM);
    assert_true(parse_effect_strength("3") == BIG);
    assert_true(parse_effect_strength("52") == (enum strength) - 1);
}

static void test_algorithm(void **state)
{
    (void)state;
    assert_true(parse_algorithm_type("linear") == LINEAR);
    assert_true(parse_algorithm_type("parallel_pixel") == PARALLEL_PIXEL);
    assert_true(parse_algorithm_type("parallel_row") == PARALLEL_ROW);
    assert_true(parse_algorithm_type("parallel_column") == PARALLEL_COLUMN);
    assert_true(parse_algorithm_type("quantum") == (enum algorithm_type) - 1);
}

static void test_type(void **state)
{
    (void)state;
    assert_true(parse_type("classic") == CLASSIC);
    assert_true(parse_type("queue") == QUEUE);
    assert_true(parse_type("kueue") == (enum type) - 1);
}

int parse_tests(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_effect),
        cmocka_unit_test(test_strength),
        cmocka_unit_test(test_algorithm),
        cmocka_unit_test(test_type),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}

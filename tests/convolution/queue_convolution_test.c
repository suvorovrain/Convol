// clang-format off
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmocka.h>
// clang-format on

#include "../../src/utils/include/io.h"
#include "utils/test_utils.h"

static void test_queue_parallel_row(void **state)
{
    (void)state;
    run_binary("image-examples/small/*.bmp", "linear", LINEAR_RESULTS, "classic");
    run_binary("image-examples/small/*.bmp", "parallel_row", PARALLEL_ROW_RESULTS, "queue");
    compare_dirs(LINEAR_RESULTS, PARALLEL_ROW_RESULTS);
    clean_dirs();

    run_binary("image-examples/big/*.bmp", "linear", LINEAR_RESULTS, "classic");
    run_binary("image-examples/big/*.bmp", "parallel_row", PARALLEL_ROW_RESULTS, "queue");
    compare_dirs(LINEAR_RESULTS, PARALLEL_ROW_RESULTS);
    clean_dirs();
}

static void test_queue_parallel_col(void **state)
{
    (void)state;
    run_binary("image-examples/small/*.bmp", "linear", LINEAR_RESULTS, "classic");
    run_binary("image-examples/small/*.bmp", "parallel_column", PARALLEL_COLUMN_RESULTS, "queue");
    compare_dirs(LINEAR_RESULTS, PARALLEL_COLUMN_RESULTS);
    clean_dirs();

    run_binary("image-examples/big/*.bmp", "linear", LINEAR_RESULTS, "classic");
    run_binary("image-examples/big/*.bmp", "parallel_column", PARALLEL_COLUMN_RESULTS, "queue");
    compare_dirs(LINEAR_RESULTS, PARALLEL_COLUMN_RESULTS);
    clean_dirs();
}

static void test_queue_parallel_pixel(void **state)
{
    (void)state;
    run_binary("image-examples/small/*.bmp", "linear", LINEAR_RESULTS, "classic");
    run_binary("image-examples/small/*.bmp", "parallel_pixel", PARALLEL_PIXEL_RESULTS, "queue");
    compare_dirs(LINEAR_RESULTS, PARALLEL_PIXEL_RESULTS);
    clean_dirs();

    run_binary("image-examples/big/*.bmp", "linear", LINEAR_RESULTS, "classic");
    run_binary("image-examples/big/*.bmp", "parallel_pixel", PARALLEL_PIXEL_RESULTS, "queue");
    compare_dirs(LINEAR_RESULTS, PARALLEL_PIXEL_RESULTS);
    clean_dirs();
}

int queue_convolution_tests(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_queue_parallel_row),
        cmocka_unit_test(test_queue_parallel_col),
        cmocka_unit_test(test_queue_parallel_pixel),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}

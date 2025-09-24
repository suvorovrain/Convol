#include "../../src/filters/filters.h"
#include "../../src/utils/include/queue.h"
#include "../../src/utils/include/subjects.h"
// clang-format off
#include <pthread.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <cmocka.h>
// clang-format on

static image_data *mock_convolution(image_data *src, convolution_filter *f)
{
    (void)f;
    return src;
}

static void test_worker_pill(void **state)
{
    (void)state;
    b_queue *qin = bq_init(2);
    b_queue *qout = bq_init(2);

    wk_params_t params = {0};
    params.convolution = mock_convolution;
    params.queue_in = qin;
    params.queue_out = qout;
    params.filter = set_filter(BLUR, SMALL);

    in_task *pill = create_in_task(PILL_TASK_ID, NULL, NULL);
    bq_enqueue(qin, pill);

    pthread_t th;
    pthread_create(&th, NULL, work, &params);
    pthread_join(th, NULL);

    free_convolution_filter(params.filter);
    bq_destroy(qin);
    bq_destroy(qout);
}

static void test_writer_pill(void **state)
{
    (void)state;
    b_queue *qout = bq_init(2);
    wr_params_t params = {0};
    params.queue_out = qout;
    params.dest_folder = "build/test-out";

    out_task *pill = create_out_task(PILL_TASK_ID, NULL, NULL);
    bq_enqueue(qout, pill);

    pthread_t th;
    pthread_create(&th, NULL, write, &params);
    pthread_join(th, NULL);

    bq_destroy(qout);
}

int subjects_tests(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_worker_pill),
        cmocka_unit_test(test_writer_pill),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}

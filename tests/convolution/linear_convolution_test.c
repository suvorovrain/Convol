
#include "../../src/algo/include/linear_convolution.h"
#include "../../src/filters/filters.h"
#include "../../src/utils/include/io.h"

// clang-format off

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>
// clang-format on

#include "utils/test_utils.h"

static const int components_count = 3;

static void test_identity_filter(void **state)
{
    (void)state;

    // synthetic
    const struct
    {
        int width;
        int height;
    } sizes[] = {{1, 1}, {2, 2}, {3, 5}, {5, 3}, {8, 7}, {16, 16}, {31, 9}};

    for (size_t i = 0; i < sizeof(sizes) / sizeof(sizes[0]); i++)
    {
        image_data *original_image =
            make_pattern_image(sizes[i].width, sizes[i].height, components_count);

        image_data *result_image = linear_convolution(original_image, &tiny_identity_filter);
        assert_images_equal(original_image, result_image);
        free_image(result_image);

        result_image = linear_convolution(original_image, &small_identity_filter);
        assert_images_equal(original_image, result_image);
        free_image(result_image);

        free_image(original_image);
    }

    // real image
    image_data *original_image = load_image("image-examples/small/hmk.bmp");
    image_data *result_image = linear_convolution(original_image, &tiny_identity_filter);
    assert_images_equal(original_image, result_image);
    free_image(original_image);
    free_image(result_image);
}

static void test_zero_filter(void **state)
{
    (void)state;

    // synthetic

    const struct
    {
        int width;
        int height;
    } sizes[] = {{1, 1}, {2, 2}, {4, 3}, {9, 7}, {32, 32}};

    for (size_t i = 0; i < sizeof(sizes) / sizeof(sizes[0]); i++)
    {
        image_data *original_image =
            make_pattern_image(sizes[i].width, sizes[i].height, components_count);

        image_data *result_image = linear_convolution(original_image, &zero_filter);
        assert_image_all_zero(result_image);
        free_image(result_image);

        free_image(original_image);
    }

    // real image
    image_data *original_image = load_image("image-examples/small/hmk.bmp");
    image_data *result_image = linear_convolution(original_image, &zero_filter);
    assert_image_all_zero(result_image);
    free_image(original_image);
    free_image(result_image);
}

static void test_zero_padding_filter(void **state)
{
    (void)state;

    convolution_filter *filter = set_filter(EMBOSS, SMALL);

    // synthetic
    const struct
    {
        int width;
        int height;
    } sizes[] = {{9, 9}, {12, 12}, {17, 11}, {32, 24}};

    for (size_t i = 0; i < sizeof(sizes) / sizeof(sizes[0]); i++)
    {
        image_data *original_image =
            make_pattern_image(sizes[i].width, sizes[i].height, components_count);

        image_data *result_image = linear_convolution(original_image, filter);
        image_data *result_image_padded = linear_convolution(original_image, &padded_emboss_filter);

        assert_images_equal(result_image, result_image_padded);

        free_image(result_image);
        free_image(result_image_padded);
        free_image(original_image);
    }
    // real image
    image_data *original_image = load_image("image-examples/small/hmk.bmp");
    image_data *result_image = linear_convolution(original_image, filter);
    image_data *result_image_padded = linear_convolution(original_image, &padded_emboss_filter);
    assert_images_equal(result_image, result_image_padded);
    free_image(original_image);
    free_image(result_image);
}

static void test_shift_right_left(void **state)
{
    (void)state;

    const struct
    {
        int width;
        int height;
    } sizes[] = {{2, 2}, {5, 3}, {8, 6}, {17, 9}};

    for (size_t i = 0; i < sizeof(sizes) / sizeof(sizes[0]); i++)
    {
        image_data *original_image =
            make_pattern_image(sizes[i].width, sizes[i].height, components_count);

        image_data *temp_image = linear_convolution(original_image, &shift_right_filter);
        image_data *result_image = linear_convolution(temp_image, &shift_left_filter);

        assert_images_equal_inner(original_image, result_image);

        free_image(temp_image);
        free_image(result_image);
        free_image(original_image);
    }
    // real image
    image_data *original_image = load_image("image-examples/small/hmk.bmp");
    image_data *temp_image = linear_convolution(original_image, &shift_right_filter);
    image_data *result_image = linear_convolution(temp_image, &shift_left_filter);

    assert_images_equal_inner(original_image, result_image);

    free_image(temp_image);
    free_image(result_image);
    free_image(original_image);
}

static void test_shift_left_right(void **state)
{
    (void)state;

    const struct
    {
        int width;
        int height;
    } sizes[] = {{2, 2}, {5, 3}, {8, 6}, {17, 9}};

    for (size_t i = 0; i < sizeof(sizes) / sizeof(sizes[0]); i++)
    {
        image_data *original_image =
            make_pattern_image(sizes[i].width, sizes[i].height, components_count);

        image_data *temp_image = linear_convolution(original_image, &shift_left_filter);
        image_data *result_image = linear_convolution(temp_image, &shift_right_filter);

        assert_images_equal_inner(original_image, result_image);

        free_image(temp_image);
        free_image(result_image);
        free_image(original_image);
    }
    // real image
    image_data *original_image = load_image("image-examples/small/hmk.bmp");
    image_data *temp_image = linear_convolution(original_image, &shift_left_filter);
    image_data *result_image = linear_convolution(temp_image, &shift_right_filter);

    assert_images_equal_inner(original_image, result_image);

    free_image(temp_image);
    free_image(result_image);
    free_image(original_image);
}

int linear_convolution_tests(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_identity_filter),     cmocka_unit_test(test_zero_filter),
        cmocka_unit_test(test_zero_padding_filter), cmocka_unit_test(test_shift_right_left),
        cmocka_unit_test(test_shift_left_right),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}

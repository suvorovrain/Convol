// clang-format off
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <libgen.h>
// clang-format on
#include "test_utils.h"

static size_t get_image_offset(const image_data *image, int x, int y, int component)
{
    return (y * image->width + x) * image->components + component;
}

image_data *make_pattern_image(int width, int height, int components_count)
{
    image_data *image = malloc(sizeof(image_data));
    image->width = width;
    image->height = height;
    image->components = components_count;
    size_t total_size = width * height * components_count;
    image->image = malloc(total_size);

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            for (int component = 0; component < components_count; component++)
            {
                unsigned int value = (x * 52 + y * 42);
                image->image[get_image_offset(image, x, y, component)] = value % 256u;
            }
        }
    }
    return image;
}

void assert_images_equal(const image_data *src_image, const image_data *result_image)
{
    assert_int_equal(src_image->width, result_image->width);
    assert_int_equal(src_image->height, result_image->height);
    assert_int_equal(src_image->components, result_image->components);

    size_t total_size = src_image->width * src_image->height * src_image->components;
    assert_memory_equal(src_image->image, result_image->image, total_size);
}

void assert_image_all_zero(const image_data *result_image)
{
    size_t total_size = result_image->width * result_image->height * result_image->components;
    for (size_t index = 0; index < total_size; index++)
    {
        assert_int_equal((int)result_image->image[index], 0);
    }
}

void assert_images_equal_inner(const image_data *src_image, const image_data *result_image)
{
    assert_int_equal(src_image->width, result_image->width);
    assert_int_equal(src_image->height, result_image->height);
    assert_int_equal(src_image->components, result_image->components);

    int width = src_image->width;
    int height = src_image->height;
    int components_count = src_image->components;
    if (width <= 2 || height <= 2)
    {
        return;
    }

    for (int y = 1; y < height - 1; y++)
    {
        for (int x = 1; x < width - 1; x++)
        {
            for (int component = 0; component < components_count; component++)
            {
                unsigned char expected_value =
                    src_image->image[get_image_offset(src_image, x, y, component)];
                unsigned char actual_value =
                    result_image->image[get_image_offset(result_image, x, y, component)];
                assert_int_equal(actual_value, expected_value);
            }
        }
    }
}

static double tiny_identity_matrix[3][3] = {{0, 0, 0}, {0, 1, 0}, {0, 0, 0}};
convolution_filter tiny_identity_filter = {
    .size = 3,
    .factor = 1.0,
    .bias = 0.0,
    .matrix =
        (double *[]){tiny_identity_matrix[0], tiny_identity_matrix[1], tiny_identity_matrix[2]}};

static double small_identity_matrix[5][5] = {
    {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}};
convolution_filter small_identity_filter = {
    .size = 5,
    .factor = 1.0,
    .bias = 0.0,
    .matrix =
        (double *[]){small_identity_matrix[0], small_identity_matrix[1], small_identity_matrix[2],
                     small_identity_matrix[3], small_identity_matrix[4]}};

static double zero_matrix[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
convolution_filter zero_filter = {.size = 3,
                                  .factor = 1.0,
                                  .bias = 0.0,
                                  .matrix =
                                      (double *[]){zero_matrix[0], zero_matrix[1], zero_matrix[2]}};

static double shift_right_matrix[3][3] = {{0, 0, 0}, {0, 0, 1}, {0, 0, 0}};
convolution_filter shift_right_filter = {
    .size = 3,
    .factor = 1.0,
    .bias = 0.0,
    .matrix = (double *[]){shift_right_matrix[0], shift_right_matrix[1], shift_right_matrix[2]}};

static double shift_left_matrix[3][3] = {{0, 0, 0}, {1, 0, 0}, {0, 0, 0}};
convolution_filter shift_left_filter = {
    .size = 3,
    .factor = 1.0,
    .bias = 0.0,
    .matrix = (double *[]){shift_left_matrix[0], shift_left_matrix[1], shift_left_matrix[2]}};

static double padded_emboss_matrix[9][9] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0},    {0, 0, 0, 0, 0, 0, 0, 0, 0},   {0, 0, -1, -1, -1, -1, 0, 0, 0},
    {0, 0, -1, -1, -1, 0, 1, 0, 0}, {0, 0, -1, -1, 0, 1, 1, 0, 0}, {0, 0, -1, 0, 1, 1, 1, 0, 0},
    {0, 0, 0, 1, 1, 1, 1, 0, 0},    {0, 0, 0, 0, 0, 0, 0, 0, 0},   {0, 0, 0, 0, 0, 0, 0, 0, 0}};
convolution_filter padded_emboss_filter = {
    .size = 9,
    .factor = 1.0,
    .bias = 128.0,
    .matrix =
        (double *[]){padded_emboss_matrix[0], padded_emboss_matrix[1], padded_emboss_matrix[2],
                     padded_emboss_matrix[3], padded_emboss_matrix[4], padded_emboss_matrix[5],
                     padded_emboss_matrix[6], padded_emboss_matrix[7], padded_emboss_matrix[8]}};

void run_binary(const char *src_dir, const char *mode, const char *out_dir, const char *type)
{
    char command[512];
    snprintf(command, sizeof(command), "./build/convol %s find_edges 2 %s %s %s", src_dir, out_dir,
             mode, type);
    int ret = system(command);
    assert_int_equal(ret, 0);
}

void compare_dirs(const char *dir1, const char *dir2)
{
    DIR *d1 = opendir(dir1);
    assert_non_null(d1);

    struct dirent *e1;
    while ((e1 = readdir(d1)) != NULL)
    {
        if (strcmp(e1->d_name, ".") == 0 || strcmp(e1->d_name, "..") == 0)
        {
            continue;
        }

        char path1[512];
        snprintf(path1, sizeof(path1), "%s/%s", dir1, e1->d_name);

        char path2[512];
        snprintf(path2, sizeof(path2), "%s/%s", dir2, e1->d_name);

        image_data *img1 = load_image(path1);
        image_data *img2 = load_image(path2);

        assert_non_null(img1);
        assert_non_null(img2);
        assert_int_equal(img1->width, img2->width);
        assert_int_equal(img1->height, img2->height);
        assert_int_equal(img1->components, img2->components);

        int width = img1->width;
        int height = img1->height;
        int comps = img1->components;

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                for (int c = 0; c < comps; c++)
                {
                    int idx = (y * width + x) * comps + c;
                    unsigned char v1 = img1->image[idx];
                    unsigned char v2 = img2->image[idx];
                    if (v1 != v2)
                    {
                        fail_msg("Mismatch in file %s at (%d,%d), comp %d: %d != %d", e1->d_name, x,
                                 y, c, v1, v2);
                    }
                }
            }
        }

        free_image(img1);
        free_image(img2);
    }

    closedir(d1);
}

void clean_dirs(void)
{
    char command[512];
    snprintf(command, sizeof(command), "rm -rf %s %s %s %s", LINEAR_RESULTS, PARALLEL_ROW_RESULTS,
             PARALLEL_COLUMN_RESULTS, PARALLEL_PIXEL_RESULTS);
    int ret = system(command);
    assert_int_equal(ret, 0);
}
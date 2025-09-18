#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include "../../../src/filters/filters.h"
#include "../../../src/utils/include/io.h"

image_data *make_pattern_image(int width, int height, int components_count);

void assert_images_equal(const image_data *src_image, const image_data *result_image);
void assert_image_all_zero(const image_data *result_image);
void assert_images_equal_inner(const image_data *src_image, const image_data *result_image);



extern convolution_filter tiny_identity_filter;
extern convolution_filter small_identity_filter;

extern convolution_filter zero_filter;

extern convolution_filter shift_right_filter;
extern convolution_filter shift_left_filter;

extern convolution_filter padded_emboss_filter;


#define LINEAR_RESULTS "./tests/test_result_linear"
#define PARALLEL_PIXEL_RESULTS "./tests/test_result_parallel_pixel"
#define PARALLEL_ROW_RESULTS "./tests/test_result_parallel_row"
#define PARALLEL_COLUMN_RESULTS "./tests/test_result_parallel_column"

void clean_dirs(void);
void compare_dirs(const char *dir1, const char *dir2);
int list_dir_entries(const char *dirpath, char ***out, size_t *count);
void run_binary(const char *src_dir, const char *mode, const char *out_dir, const char *type);

#endif

// clang-format off
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cmocka.h>
// clang-format on

#include "../../src/utils/include/io.h"

static int file_exists(const char *path)
{
    struct stat st;
    return stat(path, &st) == 0 && S_ISREG(st.st_mode);
}

static void test_path_trim_basename(void **state)
{
    (void)state;
    char *b1 = path_trim("/a/b/c.bmp");
    assert_non_null(b1);
    assert_string_equal(b1, "c.bmp");
    free(b1);

    char *b2 = path_trim("single.bmp");
    assert_non_null(b2);
    assert_string_equal(b2, "single.bmp");
    free(b2);
}

static void test_path_join(void **state)
{
    (void)state;
    char *p1 = path_join("dir/sub", "file.bmp");
    assert_non_null(p1);
    assert_string_equal(p1, "dir/sub/file.bmp");
    free(p1);
}

static void test_add_suffix(void **state)
{
    (void)state;
    char *s1 = add_suffix("pic.bmp");
    assert_non_null(s1);
    assert_string_equal(s1, "pic_edited.bmp");
    free(s1);
}

static void test_free_image_null(void **state)
{
    (void)state;
    free_image(NULL);
}

static void test_save_image(void **state)
{
    (void)state;
    image_data img = {0};
    img.width = 1;
    img.height = 1;
    img.components = 3;
    img.image = malloc(3);
    memset(img.image, 0xFF, 3);

    const char *dir = "build/test-out";
    const char *file = "out.bmp";
    char *full = path_join(dir, file);

    int rc = save_image(full, &img);
    assert_int_equal(rc, 0);
    assert_true(file_exists(full));

    unlink(full);
    free(full);
    free(img.image);
    rmdir(dir);
}

int io_tests(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_path_trim_basename), cmocka_unit_test(test_path_join),
        cmocka_unit_test(test_add_suffix),         cmocka_unit_test(test_free_image_null),
        cmocka_unit_test(test_save_image),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}

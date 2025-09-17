#include <stdlib.h>

int io_tests(void);
int parse_tests(void);
int queue_tests(void);
int subjects_tests(void);

int main(void)
{
    int res = 0;
    res += io_tests();
    res += parse_tests();
    res += queue_tests();
    res += subjects_tests();
    return res;
}

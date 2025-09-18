#include "../convolution/convolution.h"
#include "unit.h"
#include <stdlib.h>

int main(void)
{
    int res = 0;
    res += io_tests();
    res += parse_tests();
    res += queue_tests();
    res += subjects_tests();
    res += linear_convolution_tests();
    res += parallel_convolution_tests();
    res += queue_convolution_tests();
    return res;
}

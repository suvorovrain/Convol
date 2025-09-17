#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#include "filters/filters.h"
#include "utils/include/io.h"
#include "utils/include/parse.h"
#include "utils/include/queue.h"
#include "utils/include/subjects.h"

//----------------------------------------------------------------------------------------------
// Usage: ./convol <src_image> <blur/motion_blur/find_edges/emboss> <1/2/3> <dest>
// <linear/parallel_pixel/parallel_row/parallel_column/stream> <queue/classic> (default: classic)
//----------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
    input_data *input;
    input = validate_input(argc, argv);
    if (input == NULL)
    {
        return -1;
    };

    convolution_filter *filter = set_filter(input->effect_type, input->effect_strength);
    if (!filter)
    {
        return -1;
    };

    int res = 0;
    if (input->type == QUEUE)
    {
        double t0 = omp_get_wtime();
        res = queue_convolution(input, filter);
        double t1 = omp_get_wtime();
        printf("Elapsed time: %f seconds\n", t1 - t0);
    }
    else
    {
        double t0 = omp_get_wtime();
        res = classic_convolution(input, filter);
        double t1 = omp_get_wtime();
        printf("Elapsed time: %f seconds\n", t1 - t0);
    }
    if (res)
    {
        free_convolution_filter(filter);
        free_input_data(input);
        return -1;
    }

    printf("Images have been saved into %s\n", input->folder_for_store);
    free_convolution_filter(filter);
    free_input_data(input);
    return 0;
}

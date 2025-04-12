#include "vendor/stb_image.h"
#include "vendor/stb_image_write.h"

//-------------------------------------------------------------------------------
// power is a value that define the strengh of applied effect
//-------------------------------------------------------------------------------

enum power {
    TINY = 1,
    SMALL = 2,
    MEDIUM = 3,
    BIG = 4,
    HUGE = 5
};

typedef struct {
    char* src_image;
    char* effect_type;
    int effect_strength;
    char* folder_for_store;
} input_data;

input_data validate_input(int argc, char** input) {
    if (argc != 4) {

    } 
}
int main(int argc, char** argv) {
    char* source_image = argv[1];
    char* effect_type = argv[2];
    int effect_strenght = argv[3];
    char* folder_for_store = argv[4];

    return 0
}
typedef struct
{
    char *src_image;
    char *effect_type;
    int effect_strength;
    char *folder_for_store;
    char *algorithm;
} input_data;

input_data *validate_input(int argc, char **argv);

enum effect parse_effect(const char *str);
enum algorithm_type parse_algorithm_type(const char *str);



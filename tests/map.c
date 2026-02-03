#include <fsl.h>

int entry()
{
    map_t map = init_map();
    map_append(map, "name", "chris");
    map_append(map, "age", "3");

	int len = __get_size__(map->fields) / sizeof(_field);
	_printi(len), print("\n");
    for(int i = 0; i < len - 1; i++)
        print_args((string []){map->fields[i]->key, " -> ", map->fields[i]->value, "\n", 0});

    return 0;
}

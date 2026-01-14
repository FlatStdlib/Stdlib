#include "../../../headers/clibp.h"

map_t init_map(void)
{
	map_t map = allocate(0, sizeof(_map));
	if(!map)
		clibp_panic("error, unable to allocate...!");

	map->fields = allocate(0, sizeof(_field));
	map->len = 0;

	return map;
}

bool map_append(map_t map, string key, string value)
{
	if(!map || !key || !value)
		return false;

	field_t field = allocate(0, sizeof(_field));
	field->key = str_dup(key);
	field->value = str_dup(value);

	map->fields[map->len++] = field;
	map->fields = to_heap(map->fields, sizeof(_field) * (map->len + 1));

	return true;
}

string find_key(map_t map, string key)
{
	if(!map || !key)
		return NULL;

	for(int i = 0; i < map->len; i++)
	{
		if(mem_cmp(map->fields[i]->key, key, str_len(key)))
			return map->fields[i]->value;
	}

	return NULL;
}

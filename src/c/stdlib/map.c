#include "../../../headers/libbase.h"

public map_t init_map(void)
{
	map_t map = allocate(0, sizeof(_map));
	if(!map)
		lb_panic("unable to allocate...!");

	map->fields = allocate(0, sizeof(_field));
	map->len = 0;

	return map;
}

public bool map_append(map_t map, string key, string value)
{
	if(!map || !key || !value)
		return false;

	field_t field = allocate(0, sizeof(_field));
	if(!field)
		lb_panic("segfault");
	field->key = str_dup(key);
	field->value = str_dup(value);

	map->fields[map->len++] = field;
	map->fields = reallocate(map->fields, sizeof(_field) * (map->len + 1));

	return true;
}

public string find_key(map_t map, string key)
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

public fn field_destruct(field_t field)
{
	if(field->key)
		pfree(field->key, 1);

	if(field->value)
		pfree(field->value, 1);

	pfree(field, 1);
}

public fn map_destruct(map_t map)
{
	for(int i = 0; i < map->len; i++)
		field_destruct(map->fields[i]);

	pfree(map, 1);
}
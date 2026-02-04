#include <fsl.h>

public _map new_map()
{
	return (_map){
		.fields = allocate(0, sizeof(_field))
	};
}

public fn add_field(map_t map, string k, string v)
{
	map->fields[map->len++] = to_heap(&(_field){
		.key = str_dup(k), .value = str_dup(v)
		}, sizeof(_field)
	);

	map->fields = reallocate(map->fields, sizeof(_field) * (map->len + 1));
}

public fn add_fields(map_t map, string fields[][2])
{
    for(int i = 0; fields[i][0] != NULL; i++)
        add_field(map, fields[i][0], fields[i][1]);
}

public fn map__destruct(map_t map, int free_struct)
{
	if(map->fields)
	{
		for(int i = 0; i < map->len; i++)
		{
			field_destruct(map->fields[i]);
		}
	}

	if(free_struct)
		pfree(map, 1);
}

public bool entry(int argc, string argv[])
{
	_map m = new_map();

	add_fields(&m, (string [][2]){
		{"gay", "fag"},
		{"skid", "dick"},
		{NULL, NULL}
	});

	for(int i = 0; i < m.len; i++)
	{
		println(m.fields[i]->key);
	}

	map__destruct(&m, 0);
	return 0;
}

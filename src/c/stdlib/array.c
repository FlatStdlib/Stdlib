#include "../../../headers/libbase.h"

public array init_array(void)
{
	return allocate(sizeof(void *), 1);
}

public int array_count(array arr)
{
	int i, count = 0;
	for(i = 0; arr[i] != NULL; i++)
		count++;

	return count;
}

public array array_append(array arr, ptr p)
{
	if(!arr)
		return NULL;

	int c = array_count(arr);
	arr[c++] = p;
	array new_arr = to_heap(arr, sizeof(void *) * (c + 1));
	new_arr[c] = NULL;

	pfree(arr, 1);
	return new_arr;
}

public int array_contains_ptr(array arr, ptr p)
{
	if(!arr || !p)
		return -1;

	for(int i = 0; arr[i] != 0; i++)
	{
		if(arr[i] == p)
			return i;
	}

	return -1;
}

public int array_contains_str(array arr, string needle)
{
	if(!arr || !needle)
        return -1;

    for(int i = 0; arr[i] != 0; i++)
        if(mem_cmp(arr[i], needle, str_len(arr[i])))
            return i;

    return -1;
}

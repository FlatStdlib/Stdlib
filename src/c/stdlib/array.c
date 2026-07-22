#include "../../../headers/fsl.h"

public array init_array(void)
{
	return allocate(sizeof(void *), 1);
}

public array array_merge(array arr, array sub)
{
	if(!arr || !sub)
		return NULL;

	for(int i = 0; sub[i] != NULL; i++)
	{
		arr = array_append(arr, sub[i]);
	}

	return arr;
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

	pfree_array((array)arr);
	return new_arr;
}

public array array_append_insert(array arr, ptr p, int idx)
{
	if(!arr)
		return NULL;


	int c = array_count(arr);
	if(idx > c)
		return NULL;
		
	array new_arr = allocate(sizeof(void *), c + 1);
	int a = 0, b = 0;

	for(int i = 0; i < c; i++)
	{
		if(i == idx)
		{
			new_arr[a++] = p;
			continue;
		}

		new_arr[a++] = arr[b++];
	}
	new_arr[a++] = arr[b++];
	new_arr[a++] = NULL;

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

public array array_reverse(array arr)
{
	if(!arr)
		return NULL;

	int size = __get_size__(arr) / sizeof(ptr);
	array a = allocate(sizeof(ptr), size);
	for(int i = 0, c = size - 1; arr[i] != NULL; i++, c--)
		a[i] = str_dup(arr[c]);

	a[size] = NULL;

	return a;
}

public int array_contains_str(array arr, string needle)
{
	if(!arr || !needle)
        return -1;

    for(int i = 0; arr[i] != 0; i++)
		if(arr[i])
			if(str_cmp(arr[i], needle))
				return i;

    return -1;
}

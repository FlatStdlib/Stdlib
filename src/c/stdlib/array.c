#include "../../../headers/clibp.h"

array init_array(void)
{
	return allocate(0, sizeof(void *));
}

bool array_append(array arr, ptr p)
{
	if(!arr)
		return false;

	__meta__ *info = __get_meta__(arr);

	arr[info->length] = p;
	array new_arr = to_heap(arr, sizeof(void *) * (info->length + 1));

	pfree(arr, 1);
	arr = new_arr;

	return true;
}

int array_contains_ptr(array arr, ptr p)
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

int array_contains_str(sArr arr, string needle)
{
	if(!arr || !needle)
        return -1;

    for(int i = 0; arr[i] != 0; i++)
        if(mem_cmp(arr[i], needle, str_len(arr[i])))
            return i;

    return -1;
}

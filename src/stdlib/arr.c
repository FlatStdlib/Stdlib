#include "../../headers/clibp.h"

int arr_contains(arr args, str needle)
{
	if(!args || !needle)
        return -1;

    for(int i = 0; ((char *)args)[i] != 0; i++)
        if(mem_cmp(((char **)args)[i], needle, str_len(((char **)args)[i])))
            return i;

    return -1;
}

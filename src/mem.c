#include "../headers/clibp.h"

fn memzero(any ptr, size_t size)
{
	mem_set(ptr, 0, size);
}

int mem_cmp(any src, any ptr, size_t size)
{
	for(int i = 0; i < size; i++)
	{
		if(((char *)src)[i] != ((char *)ptr)[i])
			return 0;
	}

	return 1;
}

fn mem_cpy(any dest, any src, size_t size)
{
	for(int i = 0; i < size; i++)
	{
		((char *)dest)[i] = ((char *)src)[i];
	}
}

fn mem_set(any ptr, char ch, size_t size)
{
	for(int i = 0; i < size; i++)
	{
		((char *)ptr)[i] = ch;
	}
}

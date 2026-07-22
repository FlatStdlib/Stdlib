#include "../../headers/fsl.h"

public fn memzero(any p, size_t size)
{
	mem_set(p, 0, size);
}

public int mem_cmp(any src, any p, size_t size)
{
	for(int i = 0; i < size; i++)
	{
		if(((char *)src)[i] != ((char *)p)[i])
			return 0;
	}

	return 1;
}

public fn mem_cpy(any dest, any src, size_t size)
{
	for(int i = 0; i < size; i++)
	{
		((char *)dest)[i] = ((char *)src)[i];
	}
}

/* Required for variable zero '= {0};' */
#if defined(__GCC__) || defined(__TINYC__) || defined(__clang__)
	public fn memset(void *p, char ch, size_t size) { mem_set(p, ch, size); }
#endif

public fn mem_set(any p, char ch, size_t size)
{
	for(int i = 0; i < size; i++)
	{
		((char *)p)[i] = ch;
	}
}

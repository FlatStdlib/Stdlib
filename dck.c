#include <fsl.h>

fn __mem_cpy(any dest, any src, len_t sz)
{
	for(int i = 0; i < sz; i++)
		((char *)dest)[i] = ((char *)src)[i];
}

int main()
{
	long long v = 0;
	__mem_cpy((any)&v, "Hello World!\n", 13);
	((char *)&v)[12] = '\0';
	printf("-> %s", (ptr)&v);
	return 0;
}

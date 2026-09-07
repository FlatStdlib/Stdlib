#include <fsl.h>

void entry()
{
	long long v = 0;
	mem_cpy(&v, "Hello World!\n", 13);
	((char *)&v)[12] = '\0';
	_printf("-> %s", (ptr)&v);
	__syscall__(0, 0, 0, -1, -1, -1, _SYS_EXIT);
}

#include <fsl.h>

void thread_func()
{
    char fag[] = "gay\n";
    __syscall__(1, (long)fag, 4, 0, 0, 0, _SYS_WRITE);
    __syscall__(0, 0, 0, 0, 0, 0, _SYS_EXIT);
}

int entry()
{
	_start_thread((void *)thread_func, NULL);
	println("Done");
    return 0;
}


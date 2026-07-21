#include <fsl.h>

void thread_func(string q)
{
    println("HI");
    if(!q)
        fsl_panic("NULL argument");

    _printf("ptr: %p\n", q);
    __syscall__(1, (long)q, 6, -1, -1, -1, _SYS_WRITE);

    _printf("ptr: %p\n", q);
    __syscall__(0, 0, 0, 0, 0, 0, _SYS_EXIT);
}

int entry()
{
    string str = allocate(0, 10);
    str_append(str, "hello\n");
    _printf("ptr: %p\n", str);
	___start_thread((void *)thread_func, "hello\n");
    _sleep(2);
	println("Done");
    return 0;
}


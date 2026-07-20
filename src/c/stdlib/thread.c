#include "../../../headers/fsl.h"

long _start_thread(void (*fn)(void *), void *arg)
{
	void *stack = (void *)__syscall__(0, 65536, 3, 0x22, -1, 0, _SYS_MMAP);
    if ((long)stack < 0)
        return -1;


    void **sp = (void **)((char *)stack + 65536);

    *--sp = fn;
    *--sp = arg;


    long flags =
        0x100 |
        0x200 |
        0x400 |
        0x800 |
        0x10000;


    long ret;

	__syscall__(flags, (long)sp, 0, 0, 0, 0, _SYS_CLONE);
    return ret;
}
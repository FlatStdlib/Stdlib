#include "../../../headers/fsl.h"

#define CLONE_SETTLS         0x00080000

long run_thread(void *fnc, void *arg)
{
    void *stack = (void *)__syscall__(0, 65536, 3, 0x22, -1, 0, _SYS_MMAP);

    void **sp = (ptr *)((string)stack + 65536);
    sp = (ptr *)((u64)sp & ~0xF);
    sp--;

    *--sp = fnc;
    *--sp = arg;

    register void *n asm("r13") = arg;
    long ret = __syscall__(0x10f00, (long)sp, 0, 0, 0, 0, _SYS_CLONE);

    if(ret == 0)
    {
        pid_t pid = __syscall__(0, 0, 0, 0, 0, 0, _SYS_GETPID);
        _printf("Child: %d\n", (ptr)&pid);
        
        __syscall__(0, -1, -1, -1, -1, -1, _SYS_EXIT);
    }

    return ret;
}
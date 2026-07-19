#include "../../../headers/fsl.h"

thread init_thread(ptr (*fn)(), ptr arg)
{
    return (_thread){
        .fnc = fn,
        .arg = arg,
        0, 0, 0, 0, 0, 0
    };
}

static struct stack_head *allocate_stack(long size)
{
    long prot   = PROT_READ | PROT_WRITE;
    long flags  = MAP_PRIVATE | MAP_ANONYMOUS;

    long addr = __syscall__(0, size, prot, flags, -1, 0, _SYS_MMAP);

    if (addr < 0)
        return 0;

    return (struct stack_head *)(addr + size) - 1;
}


__attribute__((optimize("omit-frame-pointer"))) static long _sys_clone(struct stack_head *stack)
{
    __asm__ volatile (
        "mov   %%rdi, %%rsi\n"
        "mov   %0, %%edi\n"
        "mov   $56, %%eax\n"
        "syscall\n"
        "mov   %%rsp, %%rdi\n"
        "ret\n"
        :
        : "i"(CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD | CLONE_SYSVSEM)
        : "rax", "rcx", "rsi", "rdi", "r11", "memory"
    );
}

#include <fsl.h>
#define STACK_SIZE 65536

void child_entry(void)
{
    asm volatile(
        "pop %rdi\n"        // arg
        "pop %rax\n"        // function pointer
        "jmp *%rax\n"
    );
}

void test(void *arg)
{
    _printf("arg = %p\n", arg);
    println((char *)arg);

    __syscall__(0,0,0,0,0,0,_SYS_EXIT);
}

void clone_test(ptr arg)
{
    void *stack = (void *)__syscall__(0, STACK_SIZE, 3, 0x22, -1, 0, _SYS_MMAP);
    void **sp = (ptr *)((string)stack + 65536);
    sp = (ptr *)((u64)sp & ~0xF);
    sp--;

    string fag = str_dup("hello");
    _printf("fag = %p\n", fag);
    *--sp = test;
    *--sp = fag;

    long ret = __syscall__(CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD, (long)sp, 0, 0, 0, 0, _SYS_CLONE);

    if (ret == 0)
    {
        asm volatile("jmp child_entry");
    }

    println("parent");
}

int entry()
{
    clone_test(NULL);
    return 0;
}
/*
    -fomit-frame-pointer
*/
#include <architectures/x86_64.h>
#define THREAD_CLONE_FLAGS (CLONE_VM | CLONE_FS | CLONE_FILES | \
                            CLONE_SIGHAND | CLONE_THREAD | CLONE_SYSVSEM)

struct stack_head {
    void (*entry)(struct stack_head *);
    int join_futex;
} __attribute__((aligned(16)));

static struct stack_head *allocate_stack(long size)
{
    long prot   = PROT_READ | PROT_WRITE;
    long flags  = MAP_PRIVATE | MAP_ANONYMOUS;

    long addr = __syscall__(0, size, prot, flags, -1, 0, _SYS_MMAP);

    if (addr < 0)
        return 0;

    return (struct stack_head *)(addr + size) - 1;
}

//_attribute__((naked))
__attribute__((optimize("omit-frame-pointer")))
static long do_clone(struct stack_head *stack)
{
        __asm__ volatile (
            "mov   %%rdi, %%rsi\n"
            "mov   %0, %%edi\n"
            "mov   $56, %%eax\n"
            "syscall\n"
            "mov   %%rsp, %%rdi\n"
            "ret\n"
            :
            : "i"(THREAD_CLONE_FLAGS)
            : "rax", "rcx", "rsi", "rdi", "r11", "memory"
        );
}

static void thread_entry(struct stack_head *stack)
{
    println("HI");
    __atomic_store_n(&stack->join_futex, 1, __ATOMIC_SEQ_CST);

    __syscall__((long)&stack->join_futex, FUTEX_WAKE, 0x7fffffff, 0, 0, 0, _SYS_FUTEX);
    __syscall__(0, 0, 0, 0, 0, 0, _SYS_EXIT);
}

int entry(void)
{
    struct stack_head *stack = allocate_stack(64 * 1024);
    if (!stack)
        __syscall__(1, 0, 0, 0, 0, 0, _SYS_EXIT_GROUP);

    stack->entry      = thread_entry;
    stack->join_futex = 0;

    do_clone(stack);

    __syscall__((long)&stack->join_futex, FUTEX_WAIT, 0, 0, 0, 0, _SYS_FUTEX);

    __syscall__(0, 0, 0, 0, 0, 0, _SYS_EXIT_GROUP);
}
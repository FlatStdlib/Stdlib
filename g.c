#include <fsl.h>

#define SYS_mmap       9
#define SYS_clone     56
#define SYS_exit      60
#define SYS_exit_group 231
#define SYS_futex    202

#define PROT_READ     0x1
#define PROT_WRITE    0x2
#define MAP_PRIVATE   0x02
#define MAP_ANONYMOUS 0x20

// futex ops
#define FUTEX_WAIT    0
#define FUTEX_WAKE    1

// Thread clone flags
#define CLONE_VM      0x00000100
#define CLONE_FS      0x00000200
#define CLONE_FILES   0x00000400
#define CLONE_SIGHAND 0x00000800
#define CLONE_THREAD  0x00008000
#define CLONE_SYSVSEM 0x00004000

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

    // Return pointer to the top of the stack
    return (struct stack_head *)(addr + size) - 1;
}

__attribute__((naked))
static long do_clone(struct stack_head *stack)
{
    __asm__ volatile (
        "mov   %%rdi, %%rsi\n"           // child stack
        "mov   %0, %%edi\n"              // flags
        "mov   $56, %%eax\n"             // SYS_clone
        "syscall\n"
        "mov   %%rsp, %%rdi\n"           // pass stack_head to entry func
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
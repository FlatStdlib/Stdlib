#include <fsl.h>

#define CLONE_VM        0x00000100
#define CLONE_FS        0x00000200
#define CLONE_FILES     0x00000400
#define CLONE_SIGHAND   0x00000800

static inline long syscall6(
    long n,
    long a1, long a2, long a3,
    long a4, long a5, long a6)
{
    long ret;

    register long r10 asm("r10") = a4;
    register long r8  asm("r8")  = a5;
    register long r9  asm("r9")  = a6;

    asm volatile(
        "syscall"
        : "=a"(ret)
        : "a"(n),
          "D"(a1),
          "S"(a2),
          "d"(a3),
          "r"(r10),
          "r"(r8),
          "r"(r9)
        : "rcx", "r11", "memory"
    );

    return ret;
}

long _create_thread(void (*fn)(void *), void *arg)
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

void thread_func()
{
    char fag[] = "gay\n";
    __syscall__(1, (long)fag, 4, 0, 0, 0, _SYS_WRITE);
    __syscall__(0, 0, 0, 0, 0, 0, _SYS_EXIT);
}

int entry()
{
	_create_thread((void *)thread_func, NULL);
	println("Done");
    return 0;
}


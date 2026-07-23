#include <fsl.h>

typedef struct {
    ptr (*sa_sigaction)(int, ptr, ptr);
    u64 sa_flags;
    ptr (*sa_restorer)(void);
    u64 sa_mask;
} _sig;

ptr seg_handler(int sig, ptr info, ptr uctx)
{
    println("Caught");
}

int entry()
{
    _sig s;
    memzero(&s, sizeof(_sig));

    s.sa_sigaction = seg_handler;
    s.sa_flags = 0x00000004;

    __syscall__(11, (long)&s, 0, 8, 0, 0, _SYS_RT_SIGACTION);

    // Segfault
    ((string)_HEAP_)[_HEAP_PAGE_ + 4] = '\0';
    return 0;
}
#include <clibp.h>

#define SIGINT     2
#define SA_RESTART 0x10000000

typedef u32 sigset_t;

struct sigaction {
    void (*sa_handler)(int);
    unsigned long sa_flags;
    void (*sa_restorer)(void);
    sigset_t sa_mask;
};

fn sigrestorer(void) {
    __syscall__(0, 0, 0, 0, 0, 0, _SYS_RT_SIGRETURN);
}

fn sigint_handler(int sig) {
    static const char msg[] = " >SIGINT OK<\n";

    println((string)msg);
    __exit(0);
}

i8 entry(void) {
    struct sigaction sa;

    sa.sa_handler  = sigint_handler;
    sa.sa_flags    = SA_RESTART | 0x04000000;
    sa.sa_restorer = sigrestorer;
    sa.sa_mask     = 0;

    // __syscall__(2, (long)&sa, 0, 8, 0, 0, _SYS_RT_SIGACTION);
    __syscall(_SYS_RT_SIGACTION, 2, (long)&sa, 0, 8, 0, 0);

    for (;;) {
        __syscall__(-1, -1, -1, -1, -1, -1, _SYS_PAUSE);
    }

    return 0;
}
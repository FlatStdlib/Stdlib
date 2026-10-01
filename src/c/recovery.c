#include "../../headers/fsl.h"

struct __fsl_siginfo {
    int si_signo;
    int si_errno;
    int si_code;
    void *si_addr;
};

ptr __LAST_RECOVERABLE_ADDRESS__ = NULL;
int __SEGFAULT__ = 0;

struct __fsl_sigaction {
    void    (*handler)(int, struct __fsl_siginfo *, ptr);
    u64     flags;
    void    (*restorer)(void);
    u64     mask[1];
};

typedef unsigned long greg_t;
typedef greg_t gregset_t[23];

typedef struct
{
    gregset_t gregs;
    void *fpregs;
    unsigned long long __reserved1[8];
} mcontext_t;

typedef struct
{
    unsigned long uc_flags;
    void *uc_link;

    void *ss_sp;
    int ss_flags;
    unsigned long ss_size;

    mcontext_t uc_mcontext;

    unsigned long uc_sigmask[2];

    unsigned char __fpregs_mem[512] __attribute__((aligned(16)));
} ucontext_t;

enum v
{
    REG_R8 = 0,
    REG_R9,
    REG_R10,
    REG_R11,
    REG_R12,
    REG_R13,
    REG_R14,
    REG_R15,
    REG_RDI,
    REG_RSI,
    REG_RBP,
    REG_RBX,
    REG_RDX,
    REG_RAX,
    REG_RCX,
    REG_RSP,
    REG_RIP,
    REG_EFL,
    REG_CSGSFS,
    REG_ERR,
    REG_TRAPNO,
    REG_OLDMASK,
    REG_CR2
};

private void fsl_handler(int sig, struct __fsl_siginfo *info, ptr ctx)
{
    (void)sig;
    ucontext_t *uc = ctx;

    if(__FSL_DEBUG__) {
        _printf("ctx  = %p\n", ctx);
        _printf("RIP  = %p\n", (ptr)uc->uc_mcontext.gregs[REG_RIP]);
        _printf("RSP  = %p\n", (ptr)uc->uc_mcontext.gregs[REG_RSP]);
        _printf("NEW  = %p\n", __LAST_RECOVERABLE_ADDRESS__);
    }

    ptr p = info->si_addr;
    if(__FSL_DEBUG__)
        _printf("[ + ] Crashed @ %p\n", p);

    if(p != NULL)
    {
        if(__FSL_DEBUG__)
            println("[ + ] Recovering");

        __SEGFAULT__ = 1;
        if(__FSL_DEBUG__)
            _printf("Recovery @ %p\n", __LAST_RECOVERABLE_ADDRESS__);

        int count = 0;
        for(int i = 0; i < 120; i++)
        {
            if(((unsigned char *)__LAST_RECOVERABLE_ADDRESS__)[i] == 0x90 && ((unsigned char *)__LAST_RECOVERABLE_ADDRESS__)[i + 1] && ((unsigned char *)__LAST_RECOVERABLE_ADDRESS__)[i + 2]) {

                if(__FSL_DEBUG__) {
                    _printf("NOP @ %p\n", ((char *)__LAST_RECOVERABLE_ADDRESS__) + i);
                }

                i += 3;
                __LAST_RECOVERABLE_ADDRESS__ += i;

                if(__FSL_DEBUG__) {
                    _printf("Found Recovery @ %p\n\n", __LAST_RECOVERABLE_ADDRESS__);
                }

                break;
            }
        }

        
        if(__FSL_DEBUG__) {
            _printf("uc             = %p\n", uc);
            _printf("&gregs         = %p\n", uc->uc_mcontext.gregs);
            _printf("&REG_RIP       = %p\n", &uc->uc_mcontext.gregs[REG_RIP]);
            _printf("OLD RIP        = %p\n", (ptr)uc->uc_mcontext.gregs[REG_RIP]);
            _printf("NEW RIP        = %p\n", __LAST_RECOVERABLE_ADDRESS__);
        }

        uc->uc_mcontext.gregs[REG_RIP] = (greg_t)__LAST_RECOVERABLE_ADDRESS__;
        __LAST_RECOVERABLE_ADDRESS__ = NULL;

        if(__FSL_DEBUG__)
            _printf("AFTER RIP      = %p\n",(ptr)uc->uc_mcontext.gregs[REG_RIP]);

        return;
    }

    
    println("segfault");
    __syscall__(1, 0, 0, 0, 0, 0, _SYS_EXIT);
}

__attribute__((naked)) private void fsl_sigreturn(void)
{
    asm volatile (
        "mov $15, %%rax\n"
        "syscall\n"
        :
        :
        : "rax", "memory"
    );
}

public fn _enable_sig_handler(handler_t fnc) {
    struct __fsl_sigaction s = {
        .handler = (void *)fnc ? (void *)fnc : fsl_handler,
        .flags = 4 | 0x04000000,
        .restorer = fsl_sigreturn,
        .mask = {0}
    };

    ___syscall__(11, (long)&s, 0, 8, -1, -1, _SYS_RT_SIGACTION);
}

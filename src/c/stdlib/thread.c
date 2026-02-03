#include "../../../headers/fsl.h"

#define _NSIG       64
#define _NSIG_BPW   (8 * sizeof(unsigned long))
#define _NSIG_WORDS (_NSIG / _NSIG_BPW)
#define SA_NOCLDWAIT 0x00000002
#define SA_RESTORER  0x04000000


typedef struct {
    unsigned long sig[_NSIG_WORDS];
} sigset_t;

struct sigaction {
    void (*sa_handler)(int);
    unsigned long sa_flags;
    void (*sa_restorer)(void);
    sigset_t sa_mask;
};

static void sig_restorer(void)
{
    asm volatile (
        "mov $15, %%rax\n"   // SYS_rt_sigreturn
        "syscall\n"
        :
        :
        : "rax", "memory"
    );
}

public thread start_thread(handler_t fnc, ptr p, int wait)
{
	if(!fnc)
		return (thread){0};

	thread t = {
		fnc,
		p,
		wait,
		0,
		__syscall__(0, 0, 0, -1, -1, -1, _SYS_FORK),
		__syscall__(0, 0, 0, -1, -1, -1, _SYS_GETTID)
	};

    if(t.pid == 0)
	{
		p ? fnc(p) : fnc();
		__exit(0);
	} else if(t.pid > 0) {
		if(__FSL_DEBUG__) {
			print("Executed: "), _printi(t.pid), print("\n");
		}

		if(wait)
    		__syscall__(t.pid, 0, 0, -1, -1, -1, _SYS_WAIT4);
	} else {
		println("fork error");
	}

	t.ttid = __syscall__(0, 0, 0, -1, -1, -1, _SYS_GETTID);

	return t;
}

public fn thread_kill(thread *t)
{
	char output[100];
	__sprintf(output, "Killing thread: %d\n", (void *)&t->pid);
	if(__FSL_DEBUG__) {
		print(output);
	}

		print(output);
	if(t->pid > 0) {
		__syscall__(t->pid, 9, 0, 0, 0, 0, _SYS_KILL);
		__syscall__(t->pid, 0, 0, -1, -1, -1, _SYS_WAIT4);
	}
}

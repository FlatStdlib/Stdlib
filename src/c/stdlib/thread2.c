#include "../../../headers/fsl.h"

_thread_ init_thread(handler_t fn, ptr p, bool s, bool wait)
{
    if(!fn)
        return (_thread_){0};

    _thread_ t;
    memzero(&t, sizeof(_thread_));
    t.fnc = fn;
    t.arg = p;
    t.shared = s;
    t.wait = wait;

    if(s) {
        heap_t shared_buff = (heap_t)__sys_mmap(0, _HEAP_PAGE_, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        if(shared_buff <= 0)
            fsl_panic("unable to mmap");

        println("MMAP'd");
        mem_cpy(shared_buff, p, sizeof(p));
        t.arg = shared_buff;
        p = shared_buff;
    }

    return t;
}

public bool _run_thread(_thread_ *t, bool pass_thr_arg)
{
    if(!t)
        return false;

    t->running = true;
    t->completed = false;
    t->pid = __syscall__(0, 0, 0, 0, 0, 0, _SYS_FORK);

    if(t->pid == 0)
    {
        if(t->arg) {
            if(pass_thr_arg)
                t->fnc(t, t->arg);
            else
                t->fnc(t->arg);
        } else {
            t->fnc();
        }
    } else if(t->pid > 0)
    {
        t->ttid = t->pid;
        if(__FSL_DEBUG__)
            _printf("Thread Executing: %d\n", (void *)&t->pid);

        if(t->wait)
        {
            __syscall__(t->pid, 0, 0, 0, 0, 0, _SYS_WAIT4);
            t->running = false;
            t->completed = true;
        }

        return 1;
    } else {
        fsl_panic("[THREAD]: unable to run thread!");
    }

    return 0;
}

public fn _thread_kill(_thread_ *t, handler_t destructor, bool kill_in_thr)
{
    if(kill_in_thr)
        t->pid = __syscall__(0, 0, 0, 0, 0, 0, _SYS_GETPID);

	if(!t || t->pid <= 0)
		fsl_panic("Unable to kill thread!");

    if(t->arg)
    {
        if(destructor)
            destructor(t->arg);
    }

	if(__FSL_DEBUG__) {
        char output[100];
        _printf("Killing thread: %d\n", (void *)&t->pid);
		print(output);
	}

    char output[100];
    _printf("Killing thread: %d\n", (void *)&t->pid);
	print(output);

    if(t->shared)
    {
        memzero(t->arg, _HEAP_PAGE_);
        __syscall__((long)t->arg, _HEAP_PAGE_, 0, 0, 0, 0, _SYS_MUNMAP);
    }

	__syscall__(t->pid, 9, 0, 0, 0, 0, _SYS_KILL);
	__syscall__(t->pid, 0, 0, -1, -1, -1, _SYS_WAIT4);
	t->running = false;
	t->completed = true;
    println("KILLED");
}
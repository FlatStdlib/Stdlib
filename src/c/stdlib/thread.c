#include "../../../headers/libbase.h"

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
		if(__LB_DEBUG__) {
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
	if(__LB_DEBUG__) {
		print(output);
	}

		print(output);
	if(t->pid > 0) {
		__syscall__(t->pid, 9, 0, 0, 0, 0, _SYS_KILL);
		__syscall__(t->pid, 0, 0, -1, -1, -1, _SYS_WAIT4);
	}
}

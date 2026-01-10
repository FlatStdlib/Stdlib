#include <clibp.h>
#include <signal.h>
typedef struct
{
	ptr 	(*fnc)();
	ptr		arguments;
	i8 		wait;
	i8 		finished;
	i32		pid;
	i32		ttid;
} thread;


thread start_thread(void *(*fnc)(), ptr p, int wait)
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

	char output[100];
    if(t.pid == 0)
	{
		p ? fnc(p) : fnc();
		__exit(0);
	} else if(t.pid > 0) {
		if(__CLIBP_DEBUG__) {
			__sprintf(output, "Executed: %d\n", (void *)&t.pid);
			print(output);
		}

		if(wait)
    		__syscall__(t.pid, 0, 0, -1, -1, -1, _SYS_WAIT4);
	} else {
		println("fork error");
	}

	t.ttid = __syscall__(0, 0, 0, -1, -1, -1, _SYS_GETTID);
	print("PID: "), _printi(t.pid);

	return t;
}

void execute(char *app, char **args)
{
	if(!app || !args)
    	return;

	long pid = __syscall__(0, 0, 0, -1, -1, -1, _SYS_FORK);

    if(pid == 0)
	{
		__syscall__((long)app, (long)args, 0, -1, -1, -1, _SYS_EXECVE);
	} else if(pid > 0) {
		__syscall__(pid, 0, 0, -1, -1, -1, _SYS_WAIT4);
	} else {
		__syscall__(1, (long)"fork error\n", 7, -1, -1, -1, _SYS_WRITE);
	}
}

fn thread_kill(thread *t)
{
	if(__CLIBP_DEBUG__)
		print("Killing thread: "), _printi(t->pid), print("\n");

	print_args((char *[]){"Killing: ", (char *)t->arguments, 0});
	if(t->pid > 0) {
		__syscall__(t->pid, 9, 0, 0, 0, 0, _SYS_KILL);
		__syscall__(t->pid, 0, 0, -1, -1, -1, _SYS_WAIT4);
	}
}

void *thread_fn(char *dick)
{
	struct sleep_t ts = {1, 0};
	char buff[100];
	for(int i = 0; i < 15; i++) {
		__sprintf(buff, "[%d]: Thread Arg: %s", (void *)&i, dick);
		println(buff);
		__syscall__((long)&ts, 0, 0, 0, 0, 0, _SYS_NANOSLEEP);
	}
    __exit(0);
}

int entry()
{
//	__CLIBP_DEBUG__ = 1;
	thread first = start_thread(thread_fn, "dick", 0);
	thread second = start_thread(thread_fn, "fag", 0);

	println("Waiting");
    struct sleep_t ts = {6, 0};
    __syscall__((long)&ts, 0, 0, 0, 0, 0, _SYS_NANOSLEEP);

	println("Exiting...");
	if(first.ttid > 0)
	{
	    thread_kill(&first);
	}

	if(second.ttid > 0)
		thread_kill(&second);

   	return 0;
}

int main()
{
	return entry();
}

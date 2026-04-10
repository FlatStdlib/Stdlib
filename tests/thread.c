#include <fsl.h>

public fn *thread_fn(void *t)
{
	_printf("%p %s\n", t, t);
	struct sleep_t ts = {0, 500000000};
	for(int i = 0; i < 5; i++) {
		int *n = to_heap((void *)&i, sizeof(int));
		printi(i), print_args((string []){": Theaded Arg -> ", t, "\n", 0});
		__syscall__((long)&ts, 0, 0, 0, 0, 0, _SYS_NANOSLEEP);
		pfree(n, 1);
	}

	execute("/bin/touch", (string []){"/bin/touch", "fag.txt", NULL});
    __exit(0);
}

public fn thread_test()
{
	thread first = create_thread(thread_fn, "arg_1", 0);
	thread second = create_thread(thread_fn, "arg_2", 0);
	start_thread(first), start_thread(second);

	println("Waiting");
    struct sleep_t ts = {6, 0};
    __syscall__((long)&ts, 0, 0, 0, 0, 0, _SYS_NANOSLEEP);

	println("Exiting...");
	if(first.ttid > 0)
	    thread_kill(&first);

	if(second.ttid > 0)
		thread_kill(&second);
}

int entry()
{
	__FSL_DEBUG__ = 1;
	toggle_debug_mode();
	gthread *gt = init_thread_group();

	thread t = create_thread(thread_fn, "dick", 0);
	_thread *thr = to_heap(&t, sizeof(_thread) + 1);
	_printf("%p\n", thr->arguments);
	thread_append(gt, thr);

	pool_run(gt);
	return 0;
}


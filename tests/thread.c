#include <fsl.h>
int dick = 0;
public fn *thread_fn(thread_t t)
{
	_printf("%d\n", (void *)&dick);
	_printf("%p %s\n", t, t);
	struct sleep_t ts = {0, 500000000};
	for(int i = 0; i < 10; i++) {
		int *n = to_heap((void *)&i, sizeof(int));
		printi(i), print_args((string []){": Theaded Arg -> ", "fag", "\n", 0});
		__syscall__((long)&ts, 0, 0, 0, 0, 0, _SYS_NANOSLEEP);
		pfree(n, 1);
	}

	execute("/bin/touch", (string []){"/bin/touch", "fag.txt", NULL});
	t->completed = 1;
    // __exit(0);
	dick++;
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

	_thread_ t = init_thread(thread_fn, "dick", 1, 0);
	thread_append(gt, (thread_t)&t);
	

	pool_run(gt);

	for(int i = 0; i < 30; i++)
	{
		if(i == 5)
			mem_cpy(t.arg, "skid", 4);
		__syscall__((long)&((struct sleep_t){1, 0}), 0, 0, 0, 0, 0, _SYS_NANOSLEEP);
	}
	return 0;
}


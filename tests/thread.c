#include <fsl.h>

void *thread_fn(thread *t)
{
	println("HERE\n");
	_printf("%p\n", t->arguments);
	char *dick = "arg_1";
	struct sleep_t ts = {0, 500000000};
	for(int i = 0; i < 5; i++) {
		int *n = to_heap((void *)&i, sizeof(int));
		printi(i), print_args((string []){": Theaded Arg ->", dick, "\n", 0});
		__syscall__((long)&ts, 0, 0, 0, 0, 0, _SYS_NANOSLEEP);
		pfree(n, 1);
	}

	execute("/bin/touch", (string []){"/bin/touch", "fag.txt", NULL});
	// t->finished = 1;
	// thread_kill(t);
    __exit(0);
}

void thread_test()
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
	// t.arguments = (void *[]){&t, "dick"};
	_printf("%p\n", t.arguments);
	_thread *thr = to_heap(&t, sizeof(_thread) + 1);
	_printf("%p\n", thr);
	thread_append(gt, thr);

	pool_run(gt);
	return 0;
}


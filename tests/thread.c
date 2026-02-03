#include <fsl.h>

void *thread_fn(char *dick)
{
	struct sleep_t ts = {0, 500000000};
	for(int i = 0; i < 20; i++) {
		int *n = to_heap((void *)&i, sizeof(int));
		_printi(i), print_args((string []){": Theaded Arg ->", dick, "\n", 0});
		__syscall__((long)&ts, 0, 0, 0, 0, 0, _SYS_NANOSLEEP);
	}
    __exit(0);
}

int entry()
{
	thread first = start_thread(thread_fn, "arg_1", 0);
	thread second = start_thread(thread_fn, "arg_2", 0);

	println("Waiting");
    struct sleep_t ts = {6, 0};
    __syscall__((long)&ts, 0, 0, 0, 0, 0, _SYS_NANOSLEEP);

	println("Exiting...");
	if(first.ttid > 0)
	    thread_kill(&first);

	if(second.ttid > 0)
		thread_kill(&second);

   	return 0;
}

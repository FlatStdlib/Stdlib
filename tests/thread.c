#include <fsl.h>

void *thread_fn(thread *t)
{
	println("HERE\n");
	char *dick = "arg_1";
	struct sleep_t ts = {0, 500000000};
	for(int i = 0; i < 5; i++) {
		int *n = to_heap((void *)&i, sizeof(int));
		_printi(i), print_args((string []){": Theaded Arg ->", dick, "\n", 0});
		__syscall__((long)&ts, 0, 0, 0, 0, 0, _SYS_NANOSLEEP);
		pfree(n, 1);
	}

	execute("/bin/touch", (string []){"/bin/touch", "fag.txt", NULL});
	t->finished = 1;
	thread_kill(t);
    __exit(0);
}

void thread_test()
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
}

public gthread *init_thread_group()
{
	gthread *gt = allocate(0, sizeof(gthread));

	gt->threads = allocate(sizeof(_thread), 1);
	gt->idx = 0;

	return gt;
}

public bool thread_append(gthread *gt, thread *t)
{
	if(!gt || !t)
		return false;

	gt->threads[gt->idx++] = t;
	gt->threads = reallocate(gt->threads, sizeof(_thread) * (gt->idx + 1));

	return true;
}

void pool_run(gthread *gt)
{
	if(!gt)
		return;

	gt->running = 1;
	int total_running = 0;
	struct sleep_t s = { 0, 50000000 };
	while(gt->running != 0)
	{
		total_running = 0;
		for(int i = 0; i < gt->idx; i++)
		{
			if(!gt->threads[i]->finished)
			{
				if(!gt->threads[i]->running)
				{
					println("EXECUTED...!");
					gt->threads[i]->running = 1;
					/* Wait here so parent process can deterministically mark completion. */
					run_thread(gt->threads[i], 1);
					gt->threads[i]->finished = 1;
					gt->threads[i]->running = 0;
				}

				total_running++;
			}
		}
		printi(total_running), println(NULL);

		if(total_running == 0)
			break;

		__syscall__((long)&s, 0, 0, 0, 0, 0, _SYS_NANOSLEEP);
	}
	gt->running = 2;
}

int entry()
{
	__FSL_DEBUG__ = 1;
	toggle_debug_mode();
	gthread *gt = init_thread_group();

	_thread *t = allocate(0, sizeof(_thread));
	string dick = allocate(0, 10);
	mem_cpy(dick, "fag", 3);
	void *fag[] = {t, dick};
	t->fnc = thread_fn;
	t->arguments = fag;
	thread_append(gt, t);

	pool_run(gt);
	return 0;
}


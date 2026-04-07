#include "../../../headers/fsl.h"

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

public fn pool_run(gthread *gt)
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
		_printf("Total Threads Running: %d\n", (void *)&total_running);

		if(total_running == 0)
			break;

		__syscall__((long)&s, 0, 0, 0, 0, 0, _SYS_NANOSLEEP);
	}
	gt->running = 2;
}
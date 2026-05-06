#include <fsl.h>

typedef struct
{
    handler_t   fnc;
    ptr         arg;
    int         shared;

    int         pid;
    int         running;
    int         completed;
    int         stay_alive;

    int         lock;
    ptr         base;
} _thr;

_thr init_thread(handler_t fn, ptr arg, bool shared, bool stay_alive) {
    _thr t;
    memzero(&t, sizeof(_thr));

    t.fnc = fn;
    t.arg = arg;
    t.stay_alive = stay_alive;

    t.running = 0;
    t.completed = 0;
    t.pid = 0;

    t.lock = 0;

    if(shared) {
        heap_t shared_buff = (heap_t)__sys_mmap(0, _HEAP_PAGE_, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        if(shared_buff <= 0)
            fsl_panic("unable to mmap");

        println("MMAP'd");
        mem_cpy(shared_buff, arg, sizeof(arg));
        t.arg = shared_buff;
    }

    return t;
}

public bool toggle_lock(_thr *t)
{
    if(!t)
        return false;

    t->lock = t->lock ? 0 : 1;
    return t->lock;
}

public fn toggle_thread(_thr *t)
{
    t->running = t->running ? 0 : 1;
    t->running ? ((thread_pool *)t->base)->running_threads++ : ((thread_pool *)t->base)->running_threads--;
}

public fn toggle_complete(_thr *t)
{
    if(!t)
        return;

    t->completed = t->completed ? 0 : 1;
}

public fn thread_destruct(_thr *t)
{
    if(t->shared)
    {
        // munmap
    }
    pfree(t, 1);
}

typedef struct 
{
    int MAX_THREADS;
    
    array   threads;
    
    /* Thread Currency */
    int     enable_currency;
    int     max_running_threads;
    int     running_threads;

    /* background thread for pool */
    int     pool_running;
    int     pool_pid;
} thread_pool;

thread_pool init_pool(int MAX)
{
    thread_pool p;
    memzero(&p, sizeof(thread_pool));

    p.MAX_THREADS = MAX;
    p.threads = init_array();
    p.threadc = 0;

    p.enable_currency = 0;
    p.max_running_threads = 0;
    p.running_threads = 0;

    p.pool_running = 0;
    p.pool_pid = 0;

    return p;
}

public bool append_thread(thread_pool *p, _thr *thread)
{
    if(!p || !thread)
        return false;

    p->threads = array_append(p->threads, thread);
    return true;
}

public bool remove_thread(thread_pool *p, _thr *thread)
{
    if(!p || !thread)
        return false;

    array arr = init_array();
    int idx = 0;

    for(int i = 0; i < p->threadc; i++)
    {
        if(p->threads[i] == thread)
        {
            thread_destruct(p->threads[i]);
            p->threads[i] = NULL;
            continue;
        }

        arr = array_append(arr, p->threads[i]);
    }

    if(p->threads)
        pfree(p->threads, 1);

    p->threads = arr;
    return true;
}

public fn start_pool(thread_pool *p)
{
    p->pool_running = 1;
    while(p->pool_running != 0)
    {
        for(int i = 0; i < p->threadc; i++)
        {
            if(p->threads[i]->stay_alive)
                continue;

            if(p->threads[i]->completed) {
                toggle_thread(p->threads[i]);
                remove_thread(p->threads[i]);
                break;
            } else if(p->threads[i]->running && !p->threads[i]->completed) {
                _usleep(30000);
                continue;
            } else if(p->running_threads >= p->MAX_THREADS) {
                _usleep(30000);
                continue;
            }

            toggle_thread(p->threads[i]);
            run_thread(p->threads[i]);
            _usleep(30000);
        }

        if(p->running_threads == 0 && p->threadc == 0)
            break;

        usleep(1);
    }

    p->pool_running = 0;
}

int entry()
{

    return 0;
}
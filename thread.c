#include <fsl.h>

struct thread {
    int tid;
    void *stack;
};

struct start_info {
    void *(*fn)(void *);
    void *arg;
};

static int thread_entry(void *arg)
{
    struct start_info *info = arg;

    info->fn(info->arg);

    __syscall__(0, 0, 0, 0, 0, 0, _SYS_EXIT);
    return 0;
}

int thread_create(struct thread *t, void *(*fn)(void *), void *arg)
{
    const size_t STACK_SIZE = 1024 * 1024;

    t->stack = (void *)__sys_mmap(
        0,
        STACK_SIZE,
        PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1,
        0
    );

    struct start_info *info =
        (struct start_info *)((char *)t->stack + 4096);

    info->fn = fn;
    info->arg = arg;

    void *stack_top =
        (char *)t->stack + STACK_SIZE;

    int flags =
        0x00000100 |  // VM
        0x00000200 |  // FS
        0x00000400 |  // FILES
        0x00000800 |  // SIGHAND
        0x00010000;   // THREAD

    t->tid = (long)__syscall__(
        flags,
        (long)stack_top,
        0,
        0,
        0,
        0,
        _SYS_CLONE
    );

    if (t->tid == 0)
        thread_entry(info);

    return 0;
}

void *worker(void *arg)
{
    int id = (int)(long)arg;

    // do work
    for (int i = 0; i < 5; i++) {
        // replace with your print/syscall
        // printf("thread %d: %d\n", id, i);
    }

    return 0;
}

struct thread t1;
public int entry()
{
    thread_create(&t1, worker, (void *)1);

    // main thread continues running here
    _sleep(3);

    return 0;
}
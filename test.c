#include <fsl.h>

#define CLONE_VM      0x100
#define CLONE_FS      0x200
#define CLONE_FILES   0x400
#define CLONE_SIGHAND 0x800
#define CLONE_THREAD  0x10000

static int child(void *arg)
{
    println("child running\n");
    __exit(0);
}

int entry()
{
    uninit_mem();
    set_heap_sz(_HEAP_PAGE_ * 10);
    init_mem();

    void *stack = allocate(0, 4096);
    void *stack_top = (char*)stack + 4096;
    stack_top = (void*)((uintptr_t)stack_top & ~0xF);

    // IMPORTANT: stack grows down
    void **sp = (void **)stack_top;

    // push argument + function
    *(--sp) = (void *)0;       // arg

    int flags =
        CLONE_VM |
        CLONE_FS |
        CLONE_FILES |
        CLONE_SIGHAND |
        CLONE_THREAD;

    __syscall__(
        CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD,
        (long)stack_top,
        0, 0, 0, 0,
        56
    );

    println("parent running\n");
    _sleep(2);
    return 0;
}
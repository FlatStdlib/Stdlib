#include <fsl.h>

void test_fn(void *x)
{
    register string v asm("r13");
    _printf("received: %p\n", v);
    println(v);
    __syscall__(0, 0, 0, 0, 0, 0, _SYS_EXIT);
}

int entry()
{
    string n = allocate(0, 10);
    str_append(n, "dick\n");
    run_thread(test_fn, n);

    _sleep(2);

    return 0;
}
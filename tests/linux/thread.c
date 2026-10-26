#include <fsl.h>

void test_fn()
{
    register string v asm("r13");
    _printf("received: %p\n", v);
    print(v);

    /* Change Heap */
    _printf("%p\n", _HEAP_);
    init_mem();
    _printf("%p\n", _HEAP_);


    __syscall__(0, 0, 0, 0, 0, 0, _SYS_EXIT);
}

int entry()
{
    _printf("%p\n", _HEAP_);
    string n = allocate(0, 10);
    _printf("%p\n", n);
    str_append(n, "dick\n");

    run_thread(test_fn, n);
    _sleep(2);

    return 0;
}
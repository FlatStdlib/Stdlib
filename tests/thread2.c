#include <fsl.h>

public fn thr_test(_thread_ *thr, string p)
{
    println(p);
    _printf("[THREAD]: %p\n", p);
    _sleep(5);

    println(p);
    println("[THREAD]: Exiting...!");

    _thread_kill(thr, NULL, 1);
}

public int entry()
{
    // toggle_debug_mode();
    string foo = str_dup("fag");

    // For Shared Buffer, Must use thr.arg from here on.
    // Or just set the new pointer to 'foo'
    _thread_ thr = init_thread((handler_t)thr_test, dick, true, false);

    // Enable passing the thread struct to the thread function so it can kill itself when done
    _run_thread(&thr, true);

    _sleep(2);

    mem_cpy(thr.arg, "skid", 4);

    _sleep(8);

    println("DONE");
    return 0;
}
#include <fsl.h>
int dck = 0;

public fn next_fn()
{ println("TEST"); }
//<t:1791132661:R>
public void test()
{
    char NIG[5];
    println("lol");

    recoverable(test, NULL, NULL);
    if(__SEGFAULT__) // Recovered and Resumed Here
    {
        __LAST_RECOVERABLE_ADDRESS__ = NULL;
        println("[ + ] Recovered");
        next_fn();
        println("> Done 1");
        return;
    }

    NIG[342342343] = 123;
    next_fn();
    println("Done 1");
}

void nig()
{
    recoverable(nig, NULL, NULL);
    if(__SEGFAULT__)
    {
        println("Recovered again");
        return;
    }

    ((unsigned short *)&dck)[35235234] =  256;
}

public i8 entry()
{
    test();
    println("Finished");
    nig();
    return 0;
}
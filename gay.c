// FSL WORKS WITH GLIBC :)
#define _C_MALLOC_ALTERNATIVE
#include "fsl_gcc.h"

int entry()
{
    char *skid = "foo";
    #ifdef __GLIBC__
        printf("GLIBC: TEST : %s\n", skid);
    #else
        _printf("FSL: TEST : %s\n", skid);
    #endif
    println("lol");
    return 0;
}

/* If Compiled with GCC, Use FSL */
/* gcc script.c -lfsl */
int main()
{ return entry(); }
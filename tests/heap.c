#include <clibp.h>

#define STR_LEN 7
#define _PRESTRING_ "Testing"

int entry()
{
    int HEAP_DEBUG = 1;
    set_heap_sz(_LARGE_MEM_SZ_);
    init_mem();

    /* int array */
    iArr n = allocate(0, 11);
    for(int i = 0; i < 10; i++)
        n[i] = i;

    for(int i = 0; i < 10; i++)
       	_printi(n[i]), print("\n");

    /* str array */
    string name = allocate(0, 10);
    for(int i = 0; i < STR_LEN; i++)
        name[i] = _PRESTRING_[i];

    println((char *)name);
    return 0;
}

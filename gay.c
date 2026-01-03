/*
    clib+ - C supported script.

  2 entry point, 1 for each compiler support

  // gcc test.c -o test -lclibp
  - C       main()

  // gcc_clibp test.c -o test
  - clib+   entry()
*/
#define DISABLE_AUTO_ARCH_DET
#define CLIBP_x86_64
#define __CLIBP__

#include <stdio.h>
#include <clibp.h>

#define _STANDARD_MEM_SZ_
#include <allocator.h>

/* gcc_clibp */
entry_t entry()
{
	println("Attempting to init heap...");
	HEAP_DEBUG = 1;
	init_mem();

	print("Heap: "), printi(__is_heap_init__()), print(": "), printi(__get_total_mem_used__()), print(":"), printi(_HEAP_PAGE_SZ_), print("\n");
	str n = allocate(0, 10);
	if(n == NULL) println("ERROR ALLOCATING\n");
    println("Compiled with CLIBP");
    return 0;
}

/* gcc */
int main()
{
    printf("Compiled with GCC\n");
    return 0;
}

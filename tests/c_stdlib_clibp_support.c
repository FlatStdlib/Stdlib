/*
	clib+ and C-stdlib supported test script

  2 entry point, 1 for each compiler support

  // gcc test.c -o test -lclibp
  - C       main()

  // gcc_clibp test.c -o test
  - clib+   entry()
*/
#include <stdio.h>
#include <stdint.h>

#include <fsl.h>
#include <asm.h>

/* gcc_clibp */
int entry()
{
    /* heap test */
	println("Attempting to init heap...");

	__FSL_DEBUG__ = 1;
    int heap_check = __is_heap_init__();
	print("Heap: "), printi(heap_check),
    print(": "), printi(__get_total_mem_used__()),
    print("/"), _printi(_HEAP_PAGE_), print("\n");

	string n = allocate(0, 10);
	if(n == NULL)
        println("ERROR ALLOCATING\n");

    str_append(n, "test");

    __meta__ *info = __get_meta__(n);
    if(n[-3] == 0x7C) println("YES");
    if(info->id == 0x7C) println("YES 2");
    println(n);
    int len = str_len(n);
    _printi(len), print("\n");


	string f = allocate(0, 10);
	if(f == NULL)
        println("ERROR ALLOCATING\n");

    str_append(f, "dick");
    println(f);

    mem_set(n, 0, 10);
    mem_cpy(n, "fag", 3);
    println(n);
    println(f);

    int mem_used = __get_total_mem_used__();

    print("Heap: "), printi(heap_check),
    print(": "), _printi(mem_used),
    print("/"), _printi(_HEAP_PAGE_), print("\n");

    println("Compiled with CLIBP");
    return 0;
}

/* gcc */
int main()
{
    printf("Compiled with GCC\n");
    return 0;
}

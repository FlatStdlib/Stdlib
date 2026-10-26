#include <fsl.h>

/* heap debug, find leaks ignoring start-up command and env vars (at the beginning of heap) */
int entry()
{
    // some random op........

    /*
        Currently detecting allocated chunks

        most free'd objs are zero'd so you cannot detect frees here
    */
    println("[ALLOCATOR]: Uninitializing");

    int leaks = allocations - freed_allocations;
    _printf("Leaks: %d\n", (ptr)&leaks);
    ((string)_HEAP_)[_HEAP_PAGE_ - 1] = '\0';
    string start = (string)_HEAP_;
    string last = (((string)_HEAP_) + _HEAP_PAGE_);
    int found = 0;
    for(int i = 0; start < last; i++, start++)
    {
        if(start[0] == 0x7C)
        {
            if(found > 0)
                println("================");

            found = 1;
            println("Memory block leak found!");
            __meta__ *info = (__meta__ *)((char *)start - (sizeof(int) + sizeof(size_t)));

            string last = (start + sizeof(int) + (info->length - 1));
            int sz = __get_size__(start + sizeof(int));
            char ptr[100];
            ptr_to_str(info, ptr);

            if(last[0] == '\0')
                print("Pointer: "), print(ptr), print("Size: "), printi(sz), print(" | Data: "), println(start + sizeof(int));
        }
    }
    return 0;
}
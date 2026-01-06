#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define class typedef struct

/* 8bit Pointer with auto clean up */
static fn __clean_up__(ptr p)
{
    printf("Freeing pointer...!\n");
	free(*(void **)p);
}

#define auto_cast (__int64_t)
#define auto_free_ptr __attribute__((cleanup(__clean_up__))) __int64_t
#define str char *

class
{
    str data;
    int len;
} test;

int main()
{
    auto_free_ptr ptr = auto_cast malloc(sizeof(test));
    ((test *)ptr)->data = strdup("testing\n");
    ((test *)ptr)->len = 8;

    return 0;
}

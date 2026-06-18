#include "headers/fsl.h"

char _TEST_HEAP_[1024];
int main()
{
    mem_cpy(_TEST_HEAP_, "Gay\n", 4);
    printf(_TEST_HEAP_);
    return 0;
}
#include <libbase.h>
#include <stdio.h>

public bool entry()
{
    println("Hi");
    return 0;
}

public int main()
{
    printf("GCC\n");
    return entry();
}
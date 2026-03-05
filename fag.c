#include <fsl.h>

extern char buffer[1024];
void test_fnc();

void _start()
{
    buffer[0] = 'h';
    buffer[1] = 'i';
    buffer[2] = '\n';

    test_fnc();

    register long exit asm("eax") = 1;
    register long code asm("ebx") = 0;
    asm("int $0x80");
}
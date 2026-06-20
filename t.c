// 32bit Version; gcc -m32 -c lol.c -fno-stack-protector && ld -m elf_i386 -o t lol.o
#include <fsl.h>

int entry()
{
	char s[] = "Hello World!";
    _printf("%s\n", s);
    return 0;
}

#include <fsl.h>

__attribute__((naked)) long custom_mmap(long, long, long, long, long, long)
{
  asm volatile(
      "push %ebp\n\t"
      "mov $0xC0, %eax\n\t"
      "mov 8(%esp), %ebx\n\t"
      "mov 12(%esp), %ecx\n\t"
      "mov 16(%esp), %edx\n\t"
      "mov 20(%esp), %esi\n\t"
      "mov 24(%esp), %edi\n\t"
      "mov %[a1], %ebp\n\t"
      "int $0x80\n\t"
      "pop %ebp\n\t"
      "ret\n\t"
    :   );
}

void _start()
{
    long d = custom_mmap(0, 4092, 3, 0x22, -1, 0);
    if(d <= 0)
    {
        register long sys asm("eax") = 1;
        register long c0de asm("ebx") = 1;
        asm("int $0x80");
    }

    register long exit asm("eax") = 1;
    register long op asm("ebx") = 0;
    asm("int $0x80");
}
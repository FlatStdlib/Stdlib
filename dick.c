__attribute__((naked)) int foo(char *fag)
{
    long ret;
   __asm__ volatile(
        "push ebp\n\t"
        "mov ebp, 1\n\t"
        "mov ebp, %[a1]"
        "int 0x80\n\t"
        "pop ebp\n\t"
        : "=r"(ret)
        : [a1]"r"(fag)
        : "ebp"
    );
}

void _start()
{
    register long exit asm("eax") = 1;
    register long op asm("ebx") = 1;
    asm("int 0x80");
}
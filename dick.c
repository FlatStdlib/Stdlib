long custom_mmap(long, long, long, long, long, long);

void _start()
{
    long d = custom_mmap(0, 4092, 3, 22, -1, 0);
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
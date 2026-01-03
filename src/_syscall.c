#include "../headers/clibp.h"

#if defined(___x86___)
	long _syscall(long n, long a1, long a2, long a3, long a4, long a5, long a6)
	{
	    long ret;
	    asm volatile (
	        "int $0x80"
	        : "=a"(ret)
	        : "a"(n),
	          "b"(a1), "c"(a2), "d"(a3),
	          "S"(a4), "D"(a5)
	        : "memory"
	    );
	    return ret;
	}

    void __syscall(long syscall, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6)
    {
        register long sys asm("eax") = syscall;
		if(arg1 == -1)
		{
			asm("int $0x80");
			return;
		}

        register long a1 asm("ebx") = arg1;
        if(arg2 == -1)
        {
            asm("int $0x80");
            return;
        }

        register long a2 asm("ecx") = arg2;
        if(arg3 == -1)
        {
            asm("int $0x80");
            return;
        }

        register long a3 asm("edx") = arg3;
        if(arg4 == -1)
        {
            asm("int $0x80");
            return;
        }

        register long a4 asm("esi") = arg4;
        if(arg5 == -1)
        {
            asm("int $0x80");
            return;
        }

        register long a5 asm("edi") = arg5;
        if(arg6 == -1)
        {
            asm("int $0x80");
            return;
        }

        asm("int $0x80");
    }
#else
	long _syscall(long n, long a1, long a2, long a3, long a4, long a5, long a6)
	{
	    long ret;
	    asm volatile (
	        "mov %5, %%r10 \n"
	        "syscall"
	        : "=a"(ret)
	        : "a"(n),
	          "D"(a1),
	          "S"(a2),
	          "d"(a3),
	          "r"(a4),
	          "r"(a5),
	          "r"(a6)
	        : "rcx", "r11", "r10", "memory"
	    );

	    return ret;
	}

    void __syscall(long syscall, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6)
    {
        register long sys asm("rax") = syscall;
		if(arg1 == -1)
		{
			asm("syscall");
			return;
		}

        register long a1 asm("rdi") = arg1;
        if(arg2 == -1)
        {
            asm("syscall");
            return;
        }

        register long a2 asm("rsi") = arg2;
        if(arg3 == -1)
        {
            asm("syscall");
            return;
        }

        register long a3 asm("rdx") = arg3;
        if(arg4 == -1)
        {
            asm("syscall");
            return;
        }

        register long a4 asm("r10") = arg4;
        if(arg5 == -1)
        {
            asm("syscall");
            return;
        }

        register long a5 asm("r8") = arg5;
        if(arg6 == -1)
        {
            asm("syscall");
            return;
        }

        register long a6 asm("r9") = arg6;
        asm("syscall");
    }
#endif

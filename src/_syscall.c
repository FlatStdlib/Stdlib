#include "../headers/clibp.h"

long __syscall__(long arg1, long arg2, long arg3, long arg4, long arg5, long arg6, long sys)
{
	register long ss asm(SYSCALL_REG) = sys;
    asm(EXECUTE_SYSCALL);
}

#if defined(___x86___)
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

    long __sys_mmap(long arg1, long arg2, long arg3, long arg4, long arg5, long arg6)
    {
        return __syscall__(arg1, arg2, arg3, arg4, arg5, arg6, _SYS_MMAP);
    }
#endif

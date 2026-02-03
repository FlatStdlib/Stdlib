#include "../../headers/fsl.h"

/* Following ABI Standards Here, A Universial syscall */
public long __syscall__(long arg1, long arg2, long arg3, long arg4, long arg5, long arg6, long sys)
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
#elif defined(__x86_64__)
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

    long ___syscall__(long arg1, long arg2, long arg3, long arg4, long arg5, long arg6, long sys)
    {
        register long rax asm("rax") = sys;
        register long rdi asm("rdi") = arg1;
        register long rsi asm("rsi") = arg2;
        register long rdx asm("rdx") = arg3;
        register long r10 asm("r10") = arg4;
        register long r8  asm("r8")  = arg5;
        register long r9  asm("r9")  = arg6;

        asm("syscall");

        register long ret asm("rax");
        return rax;
    }

    long __sys_mmap(long arg1, long arg2, long arg3, long arg4, long arg5, long arg6)
    {
    	register long sys asm("rax") = _SYS_MMAP;
        register long a1 asm("rdi") = arg1;
        register long a2 asm("rsi") = arg2;
        register long a3 asm("rdx") = arg3;
        register long a4 asm("r10") = arg4;
        register long a5 asm("r8") = arg5;
        register long a6 asm("r9") = arg6;
        asm("syscall");

        long ret;
        register long check asm("rax");
        ret = check;
        if(check < 0)
            _printi(ret);

        return ret;
    }

    long __setsockopt(long arg1, long arg2, long arg3, long arg4, long arg5)
    {
    	register long sys asm("rax") = _SYS_SETSOCKOPT;
        register long a1 asm("rdi") = arg1;
        register long a2 asm("rsi") = arg2;
        register long a3 asm("rdx") = arg3;
        register long a4 asm("r10") = arg4;
        register long a5 asm("r8") = arg5;
        asm("syscall");

        long ret;
        register long check asm("rax");
        ret = check;
        if(check < 0)
            _printi(ret);

        return ret;
    }
#elif defined(__riscv)

    long __sys_mmap(long arg1, long arg2, long arg3, long arg4, long arg5, long arg6)
    {
    	register long sys asm("a7") = _SYS_MMAP;
        register long a1 asm("a0") = arg1;
        register long a2 asm("a1") = arg2;
        register long a3 asm("a2") = arg3;
        register long a4 asm("a3") = arg4;
        register long a5 asm("a4") = arg5;
        register long a6 asm("a5") = arg6;
        asm("ecall");

        long ret;
        register long check asm("a0");
        ret = check;
        if(check < 0)
            _printi(ret);

        return ret;
//        return __syscall__(arg1, arg2, arg3, arg4, arg5, arg6, _SYS_MMAP);
    }
#endif

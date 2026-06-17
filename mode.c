#include <asm.h>

int __FSL_DEBUG__ = 1;

__attribute__((optimize("omit-frame-pointer"))) long ___built_in_syscall__(long a1, long a2, long a3, long a4, long a5, long a6, long sys)
{
	register long syscall asm(__EAX__) = sys;
	register long arg1 asm(__EBX__) = a1;
	register long arg2 asm(__ECX__) = a2;
	register long arg3 asm(__EDX__) = a3;
	register long arg4 asm(__ESI__) = a4;
	register long arg5 asm(__EDI__) = a5;
	register long arg6 asm(__EBP__) = a6;
	asm(EXECUTE_SYSCALL);
}

long ___syscall__(long a1, long a2, long a3, long a4, long a5, long a6, long sys)
{
	___built_in_syscall__(a1, a2, a3, a4, a5, a6, sys);
	if(__FSL_DEBUG__)
	{
		char OUTPUT[1024];
		OUTPUT[0] = sys + '0';
		OUTPUT[1] = ' ';
		OUTPUT[2] = '|';
		OUTPUT[3] = ' ';
		OUTPUT[4] = !a1 ? '0' : a1 + '0';
		OUTPUT[5] = ' ';
		OUTPUT[6] = !a2 ? '0' : a2 + '0';
		OUTPUT[7] = ' ';
		OUTPUT[8] = !a3 ? '0' : a3 + '0';
		OUTPUT[9] = ' ';
		OUTPUT[10] = !a4 ? '0' : a4 + '0';
		OUTPUT[11] = ' ';
		OUTPUT[12] = !a5 ? '0' : a5 + '0';
		OUTPUT[13] = ' ';
		OUTPUT[14] = !a6 ? '0' : a6 + '0';
		OUTPUT[15] = '\r';
		OUTPUT[16] = '\n';
		OUTPUT[17] = ' ';
		___built_in_syscall__(1, (long)"Sys: ", 5, -1, -1, -1, _SYS_WRITE);
		___built_in_syscall__(1, (long)OUTPUT, 17, -1, -1, -1, _SYS_WRITE);
	}
}

//	no-stack-protector because the compiler relies on some builtin functions like
//	___stack_chk_fail_local() when using the stack
__attribute__((optimize("no-stack-protector"))) void _start()
{
	char n[5];
	__builtin_memcpy(n, "test\n", 5);
	___syscall__(1, (long)n, 5, -1, -1, -1, _SYS_WRITE);
	long r = ___syscall__((long)"output.txt", 0, 0, -1, -1, -1, _SYS_OPEN);
	if(r < 0)
	{
		___syscall__(1, (long)"Error\n", 6, -1, -1, -1, _SYS_WRITE);
		___syscall__(1, -1, -1, -1, -1, -1, _SYS_EXIT);
	}

	int size = ___syscall__(r, 0L, 2, -1, -1, -1, _SYS_LSEEK);
	___syscall__(r, 0L, 0, -1, -1, -1, _SYS_LSEEK);

	char buffer[4096];
	int bytes = ___syscall__(r, (long)buffer, 4096, -1, -1, -1, _SYS_READ);

	___syscall__(1, (long)buffer, bytes, -1, -1, -1, _SYS_WRITE);
	___syscall__(r, -1, -1, -1, -1, -1, _SYS_CLOSE);
	___syscall__(0, 0, 0, 0, 0, 0, _SYS_EXIT);
}

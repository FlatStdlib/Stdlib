/*
	[Stdlib] $ gcc -m32 -nostdlib -fomit-frame-pointer -fno-stack-protector mode.c -o t.o -c
	[Stdlib] $ ld -m elf_i386 t.o -o t
	[Stdlib] $ ./t
	test
	Sys: 4
	Sys: 5
	Sys: C
	Sys: C
	Sys: 3
	Sys: 4
	Sys: 6
	[Stdlib] $ 
*/
#include <asm.h>

int __FSL_DEBUG__ = 1;

void _mem_cpy(char *buffer, char *src, int sz)
{ for(int i = 0; src[i] != '\0'; i++) buffer[i] = src[i]; }

int _str_len(char *buffer)
{ int count = 0; for(; buffer[count] != '\0'; count++); }

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

static void __print(char *buff)
{
	___built_in_syscall__(1, (long)buff, _str_len(buff), -1, -1, -1, _SYS_WRITE);
}

long ___syscall__(long a1, long a2, long a3, long a4, long a5, long a6, long sys)
{
	___built_in_syscall__(a1, a2, a3, a4, a5, a6, sys);
	if(__FSL_DEBUG__)
	{
		char OUTPUT[200];
		_mem_cpy(OUTPUT, "Sys: ", 5);
		OUTPUT[6] = sys + '0';
		OUTPUT[7] = '\n';
		___built_in_syscall__(1, (long)OUTPUT, 17, -1, -1, -1, _SYS_WRITE);
	}
}

//	no-stack-protector because the compiler relies on some builtin functions like
//	___stack_chk_fail_local() when using the stack
__attribute__((optimize("no-stack-protector"))) void _start()
{
	char n[5];
	_mem_cpy(n, "test\n", 5);
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

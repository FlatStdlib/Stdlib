
long __syscall(register long a1, register long a2, register long a3, register long a4)
{
	register long sys asm("rax") = a4;
	asm("syscall");
}

void _start()
{
	__syscall(0, 0, 0, 60);
}

void entry()
{
	char n[] = "Hi\n";
	register long sys asm("rax") = 1;
	register long op asm("rdi") = 1;
	register char *buff asm("rsi") = n;
	register long sz asm("rdx") = 3;
	asm("syscall");
}

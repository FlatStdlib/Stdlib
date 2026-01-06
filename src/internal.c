#define STR_H
#include "../headers/clibp.h"
#include "../headers/asm.h"

none __exit(int code)
{
	__syscall(60, code, 0, 0, -1, -1, -1);
}

none err_n_exit(const str buff, int code)
{
	if(buff)
		print(buff);

	__syscall(60, code, 0, 0, -1, -1, -1);
}

int get_input(str dest, len_t count) {
	char BUFFER[count];
	__syscall(_SYS_READ, 0, (long)BUFFER, count, -1, -1, -1);
	register long bytes_read asm("rax");
	mem_cpy(dest, BUFFER, count);

	return bytes_read;
}

fn ptr_to_str(ptr p, char *out)
{
    static const char hex[] = "0123456789abcdef";
    uintptr_t v = (uintptr_t)p;

    out[0] = '0';
    out[1] = 'x';

    for (int i = (sizeof(uintptr_t) * 2) - 1; i >= 0; i--) {
        out[2 + ((sizeof(uintptr_t) * 2 - 1) - i)] =
            hex[(v >> (i * 4)) & 0xF];
    }

    out[2 + sizeof(uintptr_t) * 2] = '\0';
}

none print_sz(const str buffer, int sz)
{
	__syscall(_SYS_WRITE, 1, (long)buffer, sz, 0, 0, 0);
}

none printnum(int num)
{
	char BUFF[100] = {0};
	int temp = num;
}

none printc(const char ch)
{
	char BUFF[2] = {0};
	BUFF[0] = ch;
	print(BUFF);
}

none printi(int num)
{
	char BUFF[5] = {0};
	BUFF[0] = '0' + num;
	print(BUFF);
}

none _printi(int num)
{
	if(num == 0)
	{
		print("0");
		return;
	}

	int temp = num, c = 0;
	char BUFF[500] = {0};
	while(temp)
	{
		BUFF[c++] = '0' + (temp % 10);
		temp /= 10;
	}

	for(int i = 0; i < c; i++)
	{
		char t = BUFF[i], n = BUFF[--c];
		BUFF[i] = n;
		BUFF[c] = t;
	}

	print(BUFF);
}

none print(const str buff)
{
	__syscall(1, 1, (unsigned long)buff, str_len(buff), -1, -1, -1);
}

none println(const str buff)
{
	__syscall(1, 1, (unsigned long)buff, str_len(buff), -1, -1, -1);
	__syscall(1, 1, (unsigned long)"\n", 1, -1, -1, -1);
}


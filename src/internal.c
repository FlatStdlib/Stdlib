#define STR_H
#include "../headers/clibp.h"
#include "../headers/asm.h"

int __CLIBP_DEBUG__ = 0;
fn __exit(i32 code)
{
	__syscall__(code, 0, 0, -1, -1, -1, _SYS_EXIT);
}

int get_input(string dest, i32 count) {
	char BUFFER[count];
	long bytes_read = __syscall__(0, (long)BUFFER, count, -1, -1, -1, _SYS_READ);
	mem_cpy(dest, BUFFER, count);

	return bytes_read;
}

fn ptr_to_str(ptr p, string out)
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

fn print_sz(const string buffer, i32 sz)
{
	__syscall__(1, (long)buffer, sz, 0, 0, 0, _SYS_WRITE);
}

fn printc(const char ch)
{
	char BUFF[2] = {0};
	BUFF[0] = ch;
	print(BUFF);
}

fn printi(i32 num)
{
	char BUFF[5] = {0};
	BUFF[0] = '0' + num;
	print(BUFF);
}

fn _printi(int num)
{
    int temp = num, c = 0;
    char buff[150];
    if(num == 0)
    {
		print("0");
		return;
    }

    while(temp)
    {
        buff[c++] = '0' + (temp % 10);
        temp /= 10;
    }

    for(int i = 0; i < c; i++)
    {
        char t = buff[i], n = buff[--c];
        buff[i] = n;
        buff[c] = t;
    }

	print(buff);
}

string int_to_str(int num)
{
	int temp = num, c = 0;
	char buff[150];
	if(num == 0)
	{
		buff[0] = '0';
		buff[1] = '\0';
		return str_dup(buff);
	}

	while(temp)
	{
		buff[c++] = '0' + (temp % 10);
		temp /= 10;
	}

	int g = c;
	for(int i = 0; i < c; i++)
	{
		char t = buff[i], n = buff[--c];
		buff[i] = n;
		buff[c] = t;
	}

	buff[g] = '\0';
	return str_dup(buff);
}

fn print(const string buff)
{
	__syscall__(1, (unsigned long)buff, str_len(buff), -1, -1, -1, _SYS_WRITE);
}

fn println(const string buff)
{
	__syscall__(1, (unsigned long)buff, str_len(buff), -1, -1, -1, _SYS_WRITE);
	__syscall__(1, (unsigned long)"\n", 1, -1, -1, -1, _SYS_WRITE);
}

ptr to_heap(ptr p, i32 sz)
{
	ptr pointer = allocate(0, sz);
	mem_cpy(pointer, p, sz);

	((string)pointer)[sz] = '\0';
	return pointer;
}

fn __clibp_panic(string msg)
{
	print(__FILE__), print(":"), _printi(__LINE__), print(" ");
	println(msg);
	__exit(1);
}

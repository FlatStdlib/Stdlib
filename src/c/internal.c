#define STR_H
#include "../../headers/clibp.h"
#include "../../headers/asm.h"

int __CLIBP_DEBUG__ = 0;

fn toggle_debug_mode()
{
	__CLIBP_DEBUG__ = __CLIBP_DEBUG__ ? 0 : 1;
}

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

fn execute(string app, sArr args)
{
	if(!app || !args)
		return;

	long pid = __syscall__(0, 0, 0, -1, -1, -1, _SYS_FORK);

	if(pid == 0)
	{
		__syscall__((long)app, (long)args, 0, -1, -1, -1, _SYS_EXECVE);
	} else if(pid > 0) {
    	__syscall__(pid, 0, 0, -1, -1, -1, _SYS_WAIT4);
	} else {
    	__syscall__(1, (long)"fork error\n", 7, -1, -1, -1, _SYS_WRITE);
	}
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
//    if(num == 0)
//    {
//		print("0");
//		return;
//    }

    while(temp)
    {
        buff[c++] = '0' + (temp % 10);
        temp /= 10;
    }

	int i;
    for(i = 0; i < c; i++)
    {
        char t = buff[i], n = buff[--c];
        buff[i] = n;
        buff[c] = t;
    }

	print_sz(buff, i + 1);
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

fn print_args(sArr arr)
{
	for(int i = 0; arr[i] != NULL; i++)
	{
		print(arr[i]);
	}
}

ptr to_heap(ptr p, i32 sz)
{
	ptr pointer = allocate(0, sz + 1);
	if(!pointer)
		return NULL;

	mem_cpy(pointer, p, sz);
	((string)pointer)[sz] = '\0';
	return pointer;
}

fn __clibp_panic(string msg, string file, int line)
{
	print(file), print(":"), _printi(line),
	print(" -> "), println(msg);

	__exit(1);
}

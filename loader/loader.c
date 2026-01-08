/*
*
*   [ CLIBP LINKER ]
*
*   GCC-CLIBP LOADER
*   @Reason: To debug on linux raw binaries used for making OS(s)
*/
// Loader's Main Function Declaration
#include "../headers/clibp.h"
int start_up();
int entry();
/* Declare Function from build/lib.o */

/* Declare Function from build/syscall.o */
void __syscall(long syscall, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6);

__attribute__((used, externally_visible)) void __execute(char *app, char **args)
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

static int ___get_cmd_info(char *buffer) {
    #if defined(__x86__) || defined(__x86_64__)
        long fd = __syscall__((long)"/proc/self/cmdline", 0, 0, -1, -1, -1, _SYS_OPEN);
	#elif defined(__riscv)
    	long fd = __syscall__(-100, (long)"/proc/self/cmdline", 0, 0, -1, -1, _SYS_OPENAT);
	#endif
    if(fd <= 0)
    {
        return -1;
    }
    
    char BUFFER[255];
    long bytes = __syscall__(fd, (long)BUFFER, 255, -1, -1, -1, _SYS_READ);

    mem_cpy(buffer, BUFFER, bytes);

    __syscall__(_SYS_CLOSE, fd, -1, -1, -1, -1, -1);
    return bytes;
}

static int _count_char(const char *buffer, const char ch, int sz) {
    int count = 0;
    for(int i = 0; i < sz; i++)
        if(buffer[i] == ch)
            count++;

    return count;
}

static int _find_char(const char *buffer, const char ch, int sz, int match) {
    int count = 0;
    for(int i = 0; i < sz; i++) {
        if(buffer[i] == ch)
            count++;

        if(count == match)
            return i;
    }

    return -1;
}

static int ___get_args(char *argv[]) {
    int args = 0;
    char BUFFER[1024];
    int count = ___get_cmd_info(BUFFER);

    char *ptr = BUFFER;
    int test = _count_char(BUFFER, '\0', count);

    for(int i = 0, match = 0, last = 0; i < test; i++) {
        int pos = _find_char(ptr, '\0', count, match++);
        if(pos == -1)
            break;

        argv[args++] = (char *)(ptr + (pos + 1));
    }

    return args;
}

void _start() {
    char *__ARGV__[80];
    int __ARGC__ = ___get_args(__ARGV__);

	set_heap_sz(4096 * 1);
	init_mem();
    int code = entry(__ARGC__, __ARGV__);
    uninit_mem();
    __syscall(60, code, -1, -1, -1, -1, -1);
}

#include "../../headers/libbase.h"

private int get_cmd_info(string buffer)
{
    #if defined(__x86__) || defined(__x86_64__)
        long fd = __syscall__((long)"/proc/self/cmdline", 0, 0, -1, -1, -1, _SYS_OPEN);
	#elif defined(__riscv)
    	long fd = __syscall__(-100, (long)"/proc/self/cmdline", 0, 0, -1, -1, _SYS_OPENAT);
	#endif

    if(fd <= 0)
        return -1;

    char BUFFER[1024] = {0};
    long bytes = __syscall__(fd, (long)BUFFER, 255, -1, -1, -1, _SYS_READ);
    mem_cpy(buffer, BUFFER, bytes);

    __syscall__(fd, 0, 0, -1, -1, -1, _SYS_CLOSE);
    return bytes;
}

private int _count_char(const char *buffer, const char ch, int sz)
{
    int count = 0;
    for(int i = 0; i < sz; i++)
        if(buffer[i] == ch)
            count++;

    return count;
}

private int _find_char(const char *buffer, const char ch, int sz, int match)
{
    int count = 0;
    for(int i = 0; i < sz; i++) {
        if(buffer[i] == ch)
            count++;

        if(count == match)
            return i;
    }

    return -1;
}

public int get_args(char *argv[])
{
    int args = 0;
    char BUFFER[1024] = {0};
    int count = get_cmd_info(BUFFER);

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


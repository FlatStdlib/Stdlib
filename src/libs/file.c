#include "../../headers/clibp.h"

fd_t open_file(const char *filename, FILE_MODE mode, int flags)
{
	#if defined(__x86__) || defined(__x86_64__)
    	long fd = __syscall__((long)filename, (long)flags, (long)mode, -1, -1, -1, _SYS_OPEN);
	#elif defined(__riscv)
    	long fd = __syscall__(-100, (long)filename, (long)flags, (long)mode, -1, -1, _SYS_OPENAT);
	#endif

    if(fd == -2)
		clibp_panic("[ - ] Error, No file or directory");

    if(fd < 0)
        return -1;

    return fd;
}

int file_content_size(fd_t fd)
{
	long size = __syscall__(fd, 0, 2, -1, -1, -1, _SYS_LSEEK);

	__syscall__(fd, 0, 0, -1, -1, -1, _SYS_LSEEK);
	return size;
}

int file_read(fd_t fd, char *buffer, int sz)
{
    return __syscall__(fd, (long)buffer, sz, -1, -1, -1, _SYS_READ);
}

int file_write(fd_t fd, const char *buffer, len_t len)
{
    return __syscall__(fd, (long)buffer, len, -1, -1, -1, _SYS_WRITE);
}

fn file_close(fd_t fd)
{
    __syscall__(fd, -1, -1, -1, -1, -1, _SYS_CLOSE);
}

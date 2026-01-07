#include "../../headers/clibp.h"

fd_t open_file(const char *filename, FILE_MODE mode)
{
    __syscall(_SYS_OPEN, (long)filename, (long)mode, 0, -1, -1, -1);
	register long fd asm("rax");
    if(fd == -2)
		print("[ - ] Error, No file or directory\n");

    if(fd < 0)
        return -1;

    return fd;
}

int file_content_size(fd_t fd)
{
	__syscall(_SYS_LSEEK, fd, 0, 2, -1, -1, -1);
	register long sz asm("rax");
	int size = sz;

	__syscall(_SYS_LSEEK, fd, 0, 0, -1, -1, -1);
	return size;
}

int file_read(fd_t fd, char *buffer, int sz)
{
    __syscall(_SYS_READ, fd, (long)buffer, sz, -1, -1, -1);
	register long bytes asm("rax");
	return bytes;
}

int file_write(fd_t fd, const char *buffer, len_t len)
{
    __syscall(_SYS_WRITE, fd, (long)buffer, len, -1, -1, -1);
	register long chk asm("rax");
	return chk;
}

fn file_close(fd_t fd)
{
    __syscall(fd, -1, -1, -1, -1, -1, -1);
}

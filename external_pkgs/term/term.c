/*
        The start of a terminal API for clib+
*/
#include <clibp.h>

#define TCSETS              0x5402

#define SIGINT              2
#define SA_RESTART          0x10000000
typedef u32                 sigset_t;

struct sigaction {
    handler_t   (*sa_handler)(int);
    u32         sa_flags;
    fn          (*sa_restorer)(void);
    sigset_t    sa_mask;
};

struct termios {
    u32         c_iflag;
    u32         c_oflag;
    u32         c_cflag;
    u32         c_lflag;
    u8          c_line;
    u8          c_cc[32];
};

struct termios old;

fn enable_raw(void)
{
    struct termios t;
    __syscall__(0, 0x5401, (long)&old, 0, 0, 0, _SYS_IOCTL);

    t = old;

    t.c_lflag &= ~(0x0002 | 0x0008);
    __syscall__(0, 0x5402, (long)&t, 0, 0, 0, _SYS_IOCTL);

    long flags = __syscall__(0, 3, 0, 0, 0, 0, _SYS_FCNTL);
    __syscall__(0, 4, flags | 04000, 0, 0, 0, _SYS_FCNTL);
}

fn disable_raw()
{
	__syscall__(0, TCSETS, (long)&old, 0, 0, 0, _SYS_IOCTL);
}

/* Test Script */
i8 entry()
{
	char ch;
    enable_raw();
    while(1)
    {
		int bytes = __syscall__(0, (long)&ch, 1, 0, 0, 0, _SYS_READ);
		if(bytes <= 0)
			continue;

		if(ch == 'y')
		{
			disable_raw();
			return 0;
		}

		print("Input: "), printc(ch), print("\n");
	}

	return 0;
}

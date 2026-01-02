/*
    Compile and link with loader
*/
void _syscall(long, long, long, long);

static int _str_len(const char *buffer)
{
    int count = 0;
    for(int i = 0; buffer[i] != '\0'; i++)
    {
        count++;
    }

    return count;
}

static int _mem_cmp(const char *buff, const char *cmp, int sz)
{
    for(int i = 0; i < sz; i++)
    {
        if(buff[i] != cmp[i])
            return 0;
    }

    return 1;
}

int arr_contains(char **args, int argc, char *needle)
{
    if(!args || !needle)
        return -1;

    for(int i = 0; i < argc; i++) {
		if(!args[i])
			break;

        if(_mem_cmp(args[i], needle, _str_len(args[i])))
            return i;
	}

    return -1;
}

static void _mem_cpy(char *dest, const char *buffer, int len)
{
    for(int i = 0; i < len; i++)
    {
        dest[i] = buffer[i];
    }
}

int get_pid() {
    _syscall(39, 0, 0, 0);
    volatile register long sys asm("rax");

    return sys;
}

int get_cmd_info(char *buffer) {
    _syscall(2, (long)"/proc/self/cmdline", 0, 0);
    register long open asm("rax");
    if(open <= 0)
    {
        return -1;
    }

    int fd = open;
    char BUFFER[255] = {0};
    _syscall(0, fd, (long)BUFFER, 255);
    register long bts asm("rax");

    int bytes = bts;
    _mem_cpy(buffer, BUFFER, bytes);

    _syscall(3, fd, 0, 0);
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

void print(const char *buff)
{
	_syscall(1, 1, (unsigned long)buff, _str_len(buff));
}

void execute(char *app, char **args)
{
	if(!app || !args)
		return;

	int pid;
	_syscall(57, 0, 0, 0);
	register long r asm("rax");
	pid = r;

	if(pid == 0)
	{
		_syscall(59, (long)app, (long)args, 0);
	} else if(pid > 0) {
		_syscall(61, pid, 0, 0);
	} else {
		_syscall(1, 1, (long)"fork error\n", 7);
	}
}

void _start() {
    int __ARGC__ = 0;
    char *__ARGV__[100] = {0};

    char BUFFER[1024] = {0};
    int count = get_cmd_info(BUFFER);

    char *ptr = BUFFER;
    int test = _count_char(BUFFER, '\0', count);

    for(int i = 0, match = 0, last = 0; i < test; i++) {
        int pos = _find_char(ptr, '\0', count, match++);
        if(pos == -1)
            break;

        __ARGV__[__ARGC__++] = (char *)(ptr + (pos + 1));
    }

    char *SRC_CODE_FILE = __ARGV__[1];
    int src_len = _str_len(SRC_CODE_FILE);

    char COPY[255] = {0};
    _mem_cpy(COPY, SRC_CODE_FILE, src_len);
    COPY[src_len - 1] = 'o';
    char *OPT = __ARGV__[2];

	char *rm[3] = {"/usr/bin/rm", COPY, 0};
    if(SRC_CODE_FILE[src_len - 1] == 'c') {
		print("[ + ] Compiling: "), print(SRC_CODE_FILE), print(" to ");

		print(COPY), print("....!\n");

        char *n[8] = {"/usr/bin/gcc", "-ffreestanding", "-c", SRC_CODE_FILE, "-o", COPY, "-nostdlib", 0};
        execute(n[0], n);

        if(OPT[1] == 'c') {
        	_syscall(60, 0, 0, 0);
        	execute(rm[0], rm);
        }
    }

	char *OUTPUT = __ARGV__[3];
	print("[ + ] Linking to "), print(OUTPUT), print("....!\n");
    char *n[8] = {
        "/usr/bin/ld",
		"--no-relax",
        "-o",
        OUTPUT,
		COPY,
        "build/clibp.o",
		"build/x86_64.o",
        0
    };

    execute(n[0], n);
    execute(rm[0], rm);

	if(arr_contains(__ARGV__, __ARGC__, "--strip") > -1)
	{
		print("Stripping....!\n");
		char *strip[4] = {"/usr/bin/strip", "--strip-unneeded", OUTPUT, 0};
		execute(strip[0], strip);
	}

    _syscall(60, 0, 0, 0);
}

/*
    Compile and link with loader
*/
void __syscall(long, long, long, long, long, long, long);
void print(const char *buff);
int str_len(const char *buffer);
int find_str(const char *buffer, const char *needle);
int stra(char *src, const char *sub);

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

        if(_mem_cmp(args[i], needle, str_len(args[i])))
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

int get_cmd_info(char *buffer) {
    __syscall(2, (long)"/proc/self/cmdline", 0, 0, -1, -1, -1);
    register long open asm("rax");
    if(open <= 0)
    {
        return -1;
    }

    int fd = open;
    char BUFFER[255] = {0};
    __syscall(0, fd, (long)BUFFER, 255, -1, -1, -1);
    register long bts asm("rax");

    int bytes = bts;
    _mem_cpy(buffer, BUFFER, bytes);

    __syscall(3, fd, 0, 0, -1, -1, -1);
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

void execute(char *app, char **args)
{
	if(!app || !args)
		return;

	int pid;
	__syscall(57, 0, 0, 0, -1, -1, -1);
	register long r asm("rax");
	pid = r;

	if(pid == 0)
	{
		__syscall(59, (long)app, (long)args, 0, -1, -1, -1);
	} else if(pid > 0) {
		__syscall(61, pid, 0, 0, -1, -1, -1);
	} else {
		__syscall(1, 1, (long)"fork error\n", 7, -1, -1, -1);
	}
}

int __ARGC__ = 0;
char *__ARGV__[100] = {0};

void get_cmdline_args()
{
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
}

/* GCC with default flags */
#define COMPILER_TYPE_COUNT 3
char *COMPILER_TYPES[] = {
    "/usr/bin/gcc/ -nostdlib -ffreestanding -c",
    "/usr/bin/tcc -nostdlib -ffreestanding -std=c99 -c",
    0
};

void get_compiler_type_cmd(char *cmd_buffer, char *compiler)
{
    for(int i = 0; i < COMPILER_TYPE_COUNT; i++)
    {
        if(find_str((char *)((char **)COMPILER_TYPES[i])[0], compiler))
        {
            // found compiler
            char **cmd_args = ((char **)((char ***)COMPILER_TYPES)[i][1]);
            for(int arg = 0; cmd_args[arg] != 0; arg++)
            {
                stra(cmd_buffer, cmd_args[arg]);
                stra(cmd_buffer, " ");
            }
        }
    }
}

void _start() {
    get_cmdline_args();

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

        if(arr_contains(__ARGV__, __ARGC__, "--tcc") > -1) {
            print("Compiling with TCC....\n");
            char *n[9] = {"/usr/bin/tcc", "-ffreestanding", "-std=c99", "-c", SRC_CODE_FILE, "-o", COPY, "-nostdlib", 0};
            execute(n[0], n);
            execute("/usr/bin/execstack", (char *[]){"/usr/bin/execstack", "-c", COPY, 0});
        } else {
            char *n[8] = {"/usr/bin/gcc", "-ffreestanding", "-c", SRC_CODE_FILE, "-o", COPY, "-nostdlib", 0};
            execute(n[0], n);
        }

        if(OPT[1] == 'c') {
        	__syscall(60, 0, 0, 0, -1, -1, -1);
        	execute(rm[0], rm);
        }
    }

    if(arr_contains(__ARGV__, __ARGC__, "--nolink") > -1)
	{
        print("Exiting\n");
        __syscall(60, 0, 0, 0, -1, -1, -1);
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
        "loader/loader.o",
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

    __syscall(60, 0, 0, 0, -1, -1, -1);
}

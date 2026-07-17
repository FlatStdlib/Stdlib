#include <fsl.h>

extern i32 __ARGC__;
extern string __ARGV__[50];

string FSL_STDLIBS[] = {
	"/usr/lib/libfsl.a",
	"/usr/lib/libfsl_x86.a"
};

#if defined(__linux__)
    #define CC "gcc"
#else
    #error "Unsupport Platform"
#endif

const string COMPILER_FLAGS[] = {
    "/usr/bin/gcc",
    "-ffunction-sections",
    "-fdata-sections",  
    "-Wl,--gc-sections",
    "-nostdlib",
    "-ffreestanding",
    "-c",
    NULL
};

#define LD_FLAGS 4
string LD_LINKER_FLAGS[LD_FLAGS] = {
    "/usr/bin/ld",
	"--no-relax",
    "-o",
    NULL
};

const string FILES[] = {
    "src/c/_syscall.c",
    "src/c/allocator.c",
    "src/c/any.c",
    "src/c/internal.c",
    "src/c/memory.c",
    "src/c/start_up.c",
    "src/c/stdlib/int.c",
    "src/c/stdlib/char.c",
    "src/c/stdlib/string.c",
    "src/c/stdlib/array.c",
    "src/c/stdlib/map.c",
    "src/c/stdlib/file.c",
    "src/c/stdlib/socket.c",
    "src/c/stdlib/thread.c",
    // "src/c/stdlib/heap_string.c",
    NULL
};

static void __execute(char *app, char **args)
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

bool validate_c_file(string q, int sz)
{ return (q[sz - 2] == '.' && q[sz - 1] == 'c'); }

char BUILD_COMMAND[2048];
char LINK_COMMAND[2048];
public int entry(int argc, string argv[])
{
    if(argc < 3)
    {
        _printf("[ x ] Error, Invalid arguments provided\nUsage: %s <input_file> <opt> <output_file>\n", argv[0]);
        return 1;
    }

    memzero(BUILD_COMMAND, 2048);
    
    /* Add Default Command */
    str_join(BUILD_COMMAND, (array)COMPILER_FLAGS, ' ');

    string executable[50];
    memzero(executable, 50);

    char *C_FILES[1024];
    int _c_files = 0;

    /* Iterate Command Arguments */
    int cflags = 0, exec = 0;
    for(int i = 0; i < argc; i++)
    {
        int sz = str_len(argv[i]);
        if(validate_c_file(argv[i], sz))
        {
            str_append(BUILD_COMMAND, argv[i]), str_append(BUILD_COMMAND, " ");
            C_FILES[_c_files] = argv[i];
            C_FILES[_c_files][sz - 1] = 'o';
            _c_files++;
        }

        if(mem_cmp(argv[i], "-o", 2))
            exec = i + 1;

        if(mem_cmp(argv[i], "--cflags", 8))
            cflags = i + 1;
    }

    /* Add C Flags Upon --cflags */
    if(cflags > 0)
    {
        array p = (array)argv + cflags;
        str_join(BUILD_COMMAND, p, ' ');
    }

    BUILD_COMMAND[str_len(BUILD_COMMAND) - 1] = '\0';

    /* Compilation Arguments */
    int cmd_argc = 0;
    sArr cmd_args = split_string(BUILD_COMMAND, ' ', &cmd_argc);

    for(int i = 0; i < cmd_argc; i++)
    {
        if(!cmd_args[i]) break;
        _printf("[%d]: %s\r\n", (ptr)&i, cmd_args[i]);
    }

    __execute(cmd_args[0], cmd_args);

    /* Exit Upon Object File Flag Request '-c' */
    if(array_contains_str((array)argv, "-c") > -1)
    {
        println("[ + ] Object File Created");
        return 1;
    }

    _printf("'%s'\n", BUILD_COMMAND);
    return 0;
}

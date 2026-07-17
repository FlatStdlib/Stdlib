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

char BUILD_COMMAND[2048];
char LINK_COMMAND[2048];
public int entry(int argc, string argv[])
{
    if(argc < 3)
    {
        _printf("[ x ] Error, Invalid arguments provided\nUsage: %s <input_file> <opt> <output_file>\n", argv[0]);
        return 1;
    }

    string *args = argv;
    memzero(BUILD_COMMAND, 2048);
    string executable[50];
    memzero(executable, 50);

    if(mem_cmp(args[1], "-o", 2)) // fsl -o final script.c
    {
        int sz = str_len(argv[3]);
        if(args[3][sz - 3] != '.' || args[3][sz - 2] != 'c')
        {
            println("Invalid C Script provided!");
            return 1;
        }
    } else if(mem_cmp(argv[2], "-o", 2)) // fsl script.c -o final
    {
        int sz = str_len(argv[1]);
        if(args[1][sz - 3] != '.' || args[1][sz - 2] != 'c')
        {
            println("Invalid C Script provided!");
            return 1;
        }
    }

    /* Add Default Command */
    str_join(BUILD_COMMAND, (array)COMPILER_FLAGS, ' ');

    /* Iterate Command Arguments */
    int cflags = 0;
    for(int i = 0; i < argc - 2; i++)
    {
        int sz = str_len(args[i]);
        if(args[i][sz - 3] == '.' && args[i][sz - 2] == 'c')
        {
            println(args[i]);
            str_append(BUILD_COMMAND, args[i]), str_append(BUILD_COMMAND, " ");
        }

        if(mem_cmp(args[i], "--cflags", 8))
            cflags = i + 1;
    }

    array p = (array)args + cflags;
    str_join(BUILD_COMMAND, p, ' ');

    /* Detect Object File Request '-c' */
    if(array_contains_str((array)args, "-c") > -1)
    {
        println("[ + ] Object File Created");
        return 1;
    }

    println(BUILD_COMMAND);
    return 0;
}

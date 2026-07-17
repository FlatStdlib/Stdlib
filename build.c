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

const string FLAGS[] = {
    "/usr/bin/gcc",
    "-ffunction-sections",
    "-fdata-sections",  
    "-Wl,--gc-sections",
    "-nostdlib",
    "-ffreestanding",
    "-c",
    "-nostdlib",
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
public int entry(int argc, string argv[])
{
    if(argc < 3)
    {
        _printf("[ x ] Error, Invalid arguments provided\nUsage: %s <input_file> <opt> <output_file>", argv[0]);
        return 1;
    }

    memzero(BUILD_COMMAND, 2048);

    /* Add Default Command */
    str_join(BUILD_COMMAND, (array)FLAGS, ' ');
    println(BUILD_COMMAND);
    return 0;
}

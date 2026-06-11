#include <fsl.h>

typedef struct
{
    string  cc;
    array   argv;
} _build;

public _build *init_build(string cc)
{
    _build *b = allocate(0, sizeof(_build));
    b->cc = str_dup(cc);
    b->argv = init_array();
    b->argv = array_append(b->argv, str_dup(cc));

    return b;
}

#if defined(__linux__)
    #define CC "gcc"
#else
    #error "Unsupport Platform"
#endif

const string FLAGS[] = {
    "-c",
    "-nostdlib",
    "-nostdinc",
    "-ffreestanding",
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

int entry(int argc, char *argv[])
{
    return 0;
}

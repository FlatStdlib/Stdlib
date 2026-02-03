#include <fsl.h>

typedef struct
{
    string  cc;
    sArr    argv;
    i32     argc;
} _build;

public _build *init_build(string cc)
{
    _build *b = allocate(0, sizeof(_build));
    b->cc = str_dup(cc);
    b->argv = allocate(0, sizeof(string) * 2);
    b->argv[0] = str_dup(cc);
    b->argc = 1;
    
    return b;
}

public bool append_arg(_build *b, string arg)
{
    if(!b || !arg)
        return false;

    b->argv[b->argc++] = str_dup(arg);
    b->argv = reallocate(b->argv, sizeof(string) * (b->argc + 1));

    return true;
}

public bool append_args(_build *b, sArr args)
{
    if(!b || !args)
        return false;

    for(int i = 0; args[i] != NULL; i++)
        append_arg(b, args[i]);

    return true;
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

#define RawC(...) \
    __VA_ARGS__;

#define Noob(num) \
    #num;

int entry(int argc, char *argv[])
{
    RawC(
        char *fag = "SHIT BRUHS";
        _printf("%s\n", fag);
    );

    RawC(
        char *test = "TESTING THIS";
        _printf("%s\n", test);
    );
    return 0;
}
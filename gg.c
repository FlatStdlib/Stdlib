/* Cmdline Utility for GCC_CLIBP.c */
#include <clibp.h>

/* Compiler types supported with clib+ */
#define COMPILER_TYPE_COUNT 3
string COMPILER_TYPES[] = {
    "/usr/bin/gcc/ -nostdlib -ffreestanding -c",
    "/usr/bin/tcc -nostdlib -ffreestanding -std=c99 -c",
    0
};

/* returns a stack const */
string get_compiler_type_cmd(string cmd_buffer, string compiler)
{
    for(int i = 0; i < COMPILER_TYPE_COUNT; i++)
        if(find_str(COMPILER_TYPES[i], compiler) > -1)
			return COMPILER_TYPES[i];

    return 0;
}

int entry()
{
    string test = get_compiler_type_cmd(test, "gcc");
    println(test);
    return 0;
}

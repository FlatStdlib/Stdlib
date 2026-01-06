#include <clibp.h>

/* GCC with default flags */
#define COMPILER_TYPE_COUNT 3
char *COMPILER_TYPES[] = {
    "/usr/bin/gcc/ -nostdlib -ffreestanding -c",
    "/usr/bin/tcc -nostdlib -ffreestanding -std=c99 -c",
    NULL
};

void get_compiler_type_cmd(char *cmd_buffer, char *compiler)
{
    for(int i = 0; i < COMPILER_TYPE_COUNT; i++)
    {
        if(mem_cmp((str)((char **)COMPILER_TYPES[i])[0], compiler, str_len((str)((char **)COMPILER_TYPES[i])[0])))
        {
            // found compiler
            char **cmd_args = ((char **)((char ***)COMPILER_TYPES)[i][1]);
            for(int arg = 0; cmd_args[arg] != NULL; arg++)
            {
                stra(cmd_buffer, cmd_args[arg]);
                stra(cmd_buffer, " ");
            }
        }
    }
}

int entry()
{
    char *test = allocate(0, 1024);
    get_compiler_type_cmd(test, "gcc");
    println(test);
    return 0;
}
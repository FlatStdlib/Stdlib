#include <fsl.h>

public fn gop_str_append(void *n)
{
    string src = (string)((void **)n)[0];
    string needle = (string)((void **)n)[1];

    str_append(src, needle);
}

public fn gop_str_append_int(void *n)
{
    string src = (string)((void **)n)[0];
    int num = *(int *)((void **)n)[1];

    str_append_int(src, num);
}

public fn group_op(int count, void *fn[restrict count][2], int check_returns, int exit_code)
{
    for(int i = 0; i < count; i++) {
        void **args = (void **)fn[i][1];
        if(check_returns) {
            int (*fnc)() = (int (*)())fn[i][0];
            int exit = fnc(args);
            if(exit != exit_code) {
                _printf("Operation %d returned %d\n", (void *)&i, (void *)&exit);
                fsl_panic("Unable to complete group operation");
            }
        } else {
            void *(*fnc)() = (void *(*)())fn[i][0];
            fnc(args);
        }
    }
}

int entry()
{
    string n = allocate(0, 1024);
    int num = 1337;

    /*
        Only useful for group operation, for heap buffers. 
        You must know size if you have any buffer appendace
        in the group operation list
    */
    group_op(3, (void *[][2]){
        {gop_str_append, (void *[]){n, (void *)"TESTING"} },
        {gop_str_append, (void *[]){n, (void *)" THIS "}, },
        {gop_str_append_int, (void *[]){n, &num} }
    }, 0, 0);
    println(n);
    return 0;
}
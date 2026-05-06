#include "../../../headers/fsl.h"

public fn _usleep(int usec)
{
    __syscall__(
        (long)&((struct sleep_t){0, usec}), 0, 0, 0, 0, 0, _SYS_NANOSLEEP
    );
}

public fn _sleep(int sec)
{
    __syscall__(
        (long)&((struct sleep_t){sec, 0}), 0, 0, 0, 0, 0, _SYS_NANOSLEEP
    );
}
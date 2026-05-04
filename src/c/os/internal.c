#include "../../../headers/fsl.h"

public fn _sleep(int sec)
{
    __syscall__(
        (long)&((struct sleep_t){sec, 0}), 0, 0, 0, 0, 0, _SYS_NANOSLEEP
    );
}
#include "../../../headers/fsl.h"

public bool popen(popen_t *p, string *app, sArr *args, int mode)
{
    if(!p || !app || !args)
        return false;

    long pipefd[2];
    if(__syscall__((long)pipefd, 0, 0, -1, -1, -1, _SYS_PIPE) < 0)
        return false;

    long pid = __syscall__(0, 0, 0, -1, -1, -1, _SYS_FORK);
    if(pid == 0)
    {
        __syscall__(pipefd[1], 1, -1, -1, -1, -1, _SYS_DUP2);
        __syscall__(pipefd[0], 0, 0, -1, -1, -1, _SYS_CLOSE);
        __syscall__(pipefd[1], 0, 0, -1, -1, -1, _SYS_CLOSE);
        __syscall__((long)app, (long)args, 0, -1, -1, -1, _SYS_EXECVE);

        __syscall__(1, (long)"execve error\n", 13, -1, -1, -1, _SYS_WRITE);
        __syscall__(127, 0, 0, -1, -1, -1, _SYS_EXIT);
    }

    if(pid > 0)
    {
        __syscall__(pipefd[1], 0, 0, -1, -1, -1, _SYS_CLOSE);

        p->pid = pid;
        p->fd  = pipefd[0];
        return true;
    }

    __syscall__(pipefd[0], 0, 0, -1, -1, -1, _SYS_CLOSE);
    __syscall__(pipefd[1], 0, 0, -1, -1, -1, _SYS_CLOSE);

    return false;
}

public bool pclose(popen_t *p)
{
    if(!p)
        return false;

    __syscall__(p->fd, 0, 0, -1, -1, -1, _SYS_CLOSE);
    return (long)__syscall__(p->pid, 0, 0, -1, -1, -1, _SYS_WAIT4);
}

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
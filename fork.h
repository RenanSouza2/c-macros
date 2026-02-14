#ifndef __MACROS_FORK_H__
#define __MACROS_FORK_H__

#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

__attribute__((unused))
static pid_t fork_safe(void)
{
    pid_t pid = fork();
    if(pid < 0)
    {
        exit(EXIT_FAILURE);
    }

    return pid;
}

__attribute__((unused))
static pid_t waitpid_safe(pid_t pid, int *status)
{
    pid_t pid_return = waitpid(pid, status, 0);
    if(pid_return <= 0)
    {
        exit(EXIT_FAILURE);
    }

    return pid_return;
}

#endif

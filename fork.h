#ifndef __MACROS_FORK_H__
#define __MACROS_FORK_H__

#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "assert.h"

[[nodiscard, maybe_unused]]
static pid_t fork_safe()
{
    pid_t pid = fork();
    assert(pid >= 0);
    return pid;
}

[[maybe_unused]]
static pid_t waitpid_safe(pid_t pid, int *status)
{
    int _status;
    pid_t pid_return = waitpid(pid, &_status, 0);
    assert(pid_return >= 0);

    if(status)
    {
        *status = _status;
    }

    return pid_return;
}

#endif

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

[[nodiscard, maybe_unused]]
static pid_t _waitpid_safe_internal(pid_t pid, int *status)
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

[[maybe_unused]]
static void waitpid_safe(pid_t pid, int *status)
{
    assert(pid > 0);
    (void)_waitpid_safe_internal(pid, status);
}

[[nodiscard, maybe_unused]]
static pid_t waitpid_child_safe(int *status)
{
    return _waitpid_safe_internal(0, status);
}

#endif

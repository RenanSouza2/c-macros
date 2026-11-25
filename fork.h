#ifndef MACROS_FORK_H
#define MACROS_FORK_H

#include <stdint.h>
#include <sys/wait.h>
#include <unistd.h>

#if defined(__linux__)
#include <sched.h>
#elif defined(__APPLE__)
#include <mach/mach.h>
#include <mach/thread_policy.h>
#include <pthread.h>
#endif

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
    else
    {
        assert(_status == EXIT_SUCCESS);
    }

    return pid_return;
}

[[maybe_unused]]
static void fork_lock_processor([[maybe_unused]] uint64_t index)
{
#if defined(__linux__)
    cpu_set_t cpu_set;
    CPU_ZERO(&cpu_set);
    CPU_SET(index, &cpu_set);
    assert(sched_setaffinity(0, sizeof(cpu_set), &cpu_set) == 0);
#elif defined(__APPLE__)
    thread_affinity_policy_data_t policy = { .affinity_tag = (integer_t)index };
    thread_policy_set(
        pthread_mach_thread_np(pthread_self()),
        THREAD_AFFINITY_POLICY,
        (thread_policy_t)&policy,
        THREAD_AFFINITY_POLICY_COUNT
    );
#endif
}

#endif

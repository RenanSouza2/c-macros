#ifndef __TEST_H__
#define __TEST_H__

#include <stdbool.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdarg.h>
#include <time.h>

#include "assert.h"
#include "uint.h"

#define TEST_LIB printf("\n%s\t\t", __func__);

#define TEST_FN_OPEN                    \
    {                                   \
        printf("\n\t%s\t\t", __func__); \
        bool __is_main_process = true;  \
        bool __test_memory = true;      \
        uint64_t __tag = 0;

#define TEST_FN_CLOSE               \
        if(__test_memory)           \
            TEST_ASSERT_MEM_EMPTY   \
        if(!__is_main_process)      \
            exit(EXIT_SUCCESS);     \
    }

__attribute__((format(printf, 4, 5)))
void test_log_error(uint64_t __tag, uint64_t line, char const func[], char format[], ...)
{
    va_list args;
    va_start(args, format);
    printf("\n\n\tERROR TEST\t| l: " U64P() " | %s " U64P() " | ", line, func, __tag);
    vprintf(format, args);
    printf("\n\n");
    exit(EXIT_FAILURE);
}

pid_t fork_assert(uint64_t __tag, uint64_t line, char const func[], char fork_tag[])
{
    pid_t pid = fork();
    if(pid < 0)
    {
        test_log_error(__tag, line, func, "FORKING %s", fork_tag);
    }
    return pid;
}

pid_t waitpid_assert(uint64_t __tag, uint64_t line, char const func[], pid_t pid, int *status)
{
    pid_t pid_return = waitpid(pid, status, 0);
    if(pid_return <= 0)
    {
        test_log_error(__tag, line, func, "WAITPID RETURNED %d", pid_return);
    }
    return pid_return;
}

// returns true if main process
bool start_case(uint64_t __tag, uint64_t line, char const func[], bool show, uint64_t timeout_ms)
{
    if(show)
        printf("\n\t\t%s " U64P(2) "\t\t", func, __tag);

    pid_t pid = fork_assert(__tag, line, func, "TEST");
    if(pid)
    {
        int status;
        waitpid_assert(__tag, line, func, pid, &status);
        assert(status == EXIT_SUCCESS);
        return true;
    }

    pid_t pid_test = fork_assert(__tag, line, func, "TEST");
    if(pid_test == 0)
        return false;

    int status;
    if(timeout_ms)
    {
        pid_t pid_timeout = fork_assert(__tag, line, func, "TIMEOUT");
        if(pid_timeout == 0)
        {
            struct timespec spec = (struct timespec)
            {
                .tv_sec = (long)(timeout_ms / 1000),
                .tv_nsec = (long)((timeout_ms % 1000) * 1000000)
            };
            nanosleep(&spec, NULL);
            exit(EXIT_SUCCESS);
        }

        pid_t pid_return = waitpid_assert(__tag, line, func, 0, &status);
        if(pid_return == pid_timeout)
        {
            kill(pid_test, SIGKILL);
            test_log_error(__tag, line, func, "TEST TIMEOUT");
        }

        if(pid_return != pid_test)
        {
            test_log_error(__tag, line, func, "INVALID PID CAUGHT %d", pid_return);
        }

        kill(pid_timeout, SIGKILL);
    }
    else
    {
        waitpid_assert(__tag, line, func, pid_test, &status);
    }

    if(status != EXIT_SUCCESS)
    {
        test_log_error(__tag, line, func, "ERROR IN TEST EXECUTION ");
    }
    exit(EXIT_SUCCESS);
}



#define TEST_CASE_OPEN_TIMEOUT(TAG, TIMEOUT)                                        \
    if(__is_main_process)                                                           \
    {                                                                               \
        __tag = (uint64_t)(TAG);                                                    \
        __is_main_process = start_case(__tag, __LINE__, __func__, show, TIMEOUT);   \
        if(!__is_main_process)                                                      \
        {

#define TEST_CASE_OPEN(TAG) TEST_CASE_OPEN_TIMEOUT(TAG, TEST_CASE_TIMEOUT_MS)

#define TEST_CASE_CLOSE \
        }               \
    }

pid_t start_revert(uint64_t __tag, uint64_t line, char const func[])
{
    pid_t pid = fork();
    if(pid < 0)
    {
        test_log_error(__tag, line, func, "ERROR FORKING");
    }
    if(pid)
    {
        int status;
        waitpid_assert(__tag, line, func, pid, &status);
        if(status == EXIT_SUCCESS)
        {
            test_log_error(__tag, line, func, "TEST EXPECTED TO REVERT BUT DIDN'T");
        }
    }
    else
    {
        if(
            freopen("/dev/null", "w", stderr) == NULL ||
            freopen("/dev/null", "w", stdout) == NULL
        )
        {
            printf("\n\n\tERROR REDIRECTING STD BUFFERS\n\n");
            exit(EXIT_SUCCESS);
        }
        struct timespec spec = (struct timespec) {0};
        nanosleep(&spec, NULL);
    }
    return pid;
}

#define TEST_REVERT_OPEN                                    \
    {                                                       \
        if(start_revert(__tag, __LINE__, __func__) == 0)    \
        {

#define TEST_REVERT_CLOSE       \
            exit(EXIT_SUCCESS); \
        }                       \
        __test_memory = false;  \
    }

#define ARG_OPEN(...) __VA_ARGS__

#endif

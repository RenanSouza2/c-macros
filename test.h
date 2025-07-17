#ifndef __TEST_H__
#define __TEST_H__

#include <stdbool.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdarg.h>

#include "assert.h"

#define TEST_LIB printf("\n%s\t\t", __func__);

#define TEST_FN_OPEN                    \
    {                                   \
        printf("\n\t%s\t\t", __func__); \
        bool __is_main_process = true;  \
        bool __test_memory = true;      \
        int __tag = 0;

#define TEST_FN_CLOSE               \
        if(__test_memory)           \
            TEST_ASSERT_MEM_EMPTY   \
        if(!__is_main_process)      \
            exit(EXIT_SUCCESS);     \
    }

void test_log_error(int __tag, int line, char const func[], char format[], ...)
{
    va_list args;
    va_start(args, format);
    printf("\n\n\tERROR TEST\t| l:%d | %s %d | ", line, func, __tag);
    vprintf(format, args);
    printf("\n\n");
    exit(EXIT_FAILURE);
}

pid_t fork_assert(int __tag, int line, char const func[], char fork_tag[])
{
    pid_t pid = fork();
    if(pid < 0)
    {
        test_log_error(__tag, line, func, "FORKING %s", fork_tag);
    }
    return pid;
}

pid_t waitpid_assert(int __tag, int line, char const func[], pid_t pid, int *status)
{
    pid_t pid_return = waitpid(pid, status, 0);
    if(pid_return <= 0)
    {
        test_log_error(__tag, line, func, "WAITPID RETURNED %d", pid_return);
    }
    return pid_return;
}

// returns true if main process
bool start_case(int __tag, int line, char const func[], bool show)
{
    if(show)
        printf("\n\t\t%s %2d\t\t", func, __tag);

    pid_t pid = fork_assert(__tag, line, func, "TEST");
    if(pid)
    {
        int status;
        waitpid_assert(__tag, line, func, pid, &status);
        assert(status == EXIT_SUCCESS);
        return true;
    }

    int pid_test = fork_assert(__tag, line, func, "TEST");
    if(pid_test == 0)
        return false;

    int status;
    if(TEST_CASE_TIMEOUT_MS)
    {
        pid_t pid_timeout = fork_assert(__tag, line, func, "TIMEOUT");
        if(pid_timeout == 0)
        {
            usleep(TEST_CASE_TIMEOUT_MS * 1000);
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

#define TEST_CASE_OPEN(TAG)                                                 \
    if(__is_main_process)                                                   \
    {                                                                       \
        __tag = (int)(TAG);                                                 \
        __is_main_process = start_case(__tag, __LINE__, __func__, show);    \
        if(!__is_main_process)                                              \
        {

#define TEST_CASE_CLOSE \
        }               \
    }

pid_t start_revert(int __tag, int line, char const func[])
{
    int pid = fork();
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
        usleep(0);
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

#ifndef __TEST_H__
#define __TEST_H__

#include <stdbool.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdarg.h>

#include "./assert.h"

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

void test_log_error(int __tag, char format[], ...)
{
    va_list args;
    va_start(args, format);
    printf("\n\n\tERROR TEST\t| l:%d | %s %d | ", __LINE__, __func__, __tag);
    vprintf(format, args);
    printf("\n\n");
    exit(EXIT_FAILURE);
}

bool start_case(bool show, int __tag)
{
    if(show)
        printf("\n\t\t%s %2d\t\t", __func__, __tag);
    int pid_test = fork();
    if(pid_test < 0)
    {
        test_log_error(__tag, "FORKING TEST");
    }
    if(pid_test)
    {
        pid_t pid_timeout = fork();
        if(pid_timeout < 0)
        {
            kill(pid_test, SIGKILL);
            test_log_error(__tag, "FORKING TIMEOUT");
        }
        if(pid_timeout == 0)
        {
            usleep(TEST_CASE_TIMEOUT_MS * 1000);
            exit(EXIT_SUCCESS);
        }
        int status;
        pid_t pid_return = waitpid(0, &status, 0);
        if(pid_return < 0)
        {
            test_log_error(__tag, "WAITPID RETURNED %d", pid_return);
        }
        if(pid_return == pid_timeout)
        {
            kill(pid_test, SIGKILL);
            test_log_error(__tag, "TEST TIMEOUT");
        }
        if(pid_return != pid_test)
        {
            test_log_error(__tag, "INVALID PID CAUGHT %d", pid_return);
        }
        kill(pid_timeout, SIGKILL);
        waitpid(pid_timeout, NULL, 0);
        if(status != EXIT_SUCCESS)
        {
            test_log_error(__tag, "ERROR IN TEST EXECUTION ");
        }
    }
    return pid_test != 0;
}

#define TEST_CASE_OPEN(TAG)                             \
    if(__is_main_process)                               \
    {                                                   \
        __tag = (int)(TAG);                             \
        __is_main_process = start_case(show, __tag);    \
        if(!__is_main_process)                          \
        {                                               \

#define TEST_CASE_CLOSE \
        }               \
    }

pid_t start_revert(int __tag)
{
    int pid = fork();
    if(pid < 0)
    {
        test_log_error(__tag, "ERROR FORKING");
    }
    if(pid)
    {
        int status;
        if(waitpid(pid, &status, 0) < 0)
        {
            test_log_error(__tag, "WAITPID RETURNED INVALID");
        }
        if(status == EXIT_SUCCESS)
        {
            test_log_error(__tag, "TEST EXPECTED TO REVERT BUT DIDN'T");
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

#define TEST_REVERT_OPEN                \
    {                                   \
        if(start_revert(__tag) == 0)    \
        {

#define TEST_REVERT_CLOSE       \
            exit(EXIT_SUCCESS); \
        }                       \
        __test_memory = false;  \
    }

#define ARG_OPEN(...) __VA_ARGS__

#endif

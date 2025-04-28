#ifndef __TEST_H__
#define __TEST_H__

#include <stdbool.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

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

#define TEST_LOG_ERROR(...)                                                         \
    {                                                                               \
        printf("\n\n\tERROR TEST\t| l:%d | %s %d | ", __LINE__, __func__, __tag);   \
        printf(__VA_ARGS__);                                                        \
        printf("\n\n");                                                             \
        exit(EXIT_FAILURE);                                                         \
    }

#define TEST_CASE_OPEN(TAG)                                         \
    if(__is_main_process)                                           \
    {                                                               \
        __tag = (int)(TAG);                                         \
        if(show)                                                    \
            printf("\n\t\t%s %2d\t\t", __func__, __tag);            \
        int pid_test = fork();                                      \
        assert(pid_test >= 0);                                      \
        if(pid_test < 0)                                            \
        {                                                           \
            TEST_LOG_ERROR("FORKING TEST")                          \
        }                                                           \
        if(pid_test)                                                \
        {                                                           \
            pid_t pid_timeout = fork();                             \
            if(pid_timeout < 0)                                     \
            {                                                       \
                kill(pid_test, SIGKILL);                            \
                TEST_LOG_ERROR("FORKING TIMEOUT")                   \
            }                                                       \
            if(pid_timeout == 0)                                    \
            {                                                       \
                sleep(TEST_CASE_TIMEOUT);                                \
                exit(EXIT_SUCCESS);                                 \
            }                                                       \
            int status;                                             \
            pid_t pid_return = waitpid(0, &status, 0);              \
            assert(pid_return > 0);                                 \
            if(pid_return == pid_timeout)                           \
            {                                                       \
                kill(pid_test, SIGKILL);                            \
                TEST_LOG_ERROR("TEST TIMEOUT")                      \
            }                                                       \
            if(pid_return != pid_test)                              \
            {                                                       \
                TEST_LOG_ERROR("INVALID PID CAUGHT %d", pid_return) \
            }                                                       \
            kill(pid_timeout, SIGKILL);                             \
            waitpid(pid_timeout, NULL, 0);                          \
            if(status != EXIT_SUCCESS)                              \
            {                                                       \
                TEST_LOG_ERROR("ERROR IN TEST EXECUTION ")          \
            }                                                       \
        }                                                           \
        else                                                        \
        {                                                           \
            __is_main_process = false;

#define TEST_CASE_CLOSE \
        }               \
    }

#define TEST_REVERT_OPEN                                                \
    {                                                                   \
        int pid = fork();                                               \
        assert(pid >= 0);                                               \
        if(pid < 0)                                                     \
        {                                                               \
            TEST_LOG_ERROR("ERROR FORKING");                            \
        }                                                               \
        if(pid)                                                         \
        {                                                               \
            int status;                                                 \
            assert(waitpid(pid, &status, 0) >= 0);                      \
            if(status == EXIT_SUCCESS)                                  \
            {                                                           \
                TEST_LOG_ERROR("TEST EXPECTED TO REVERT BUT DIDN'T");   \
            }                                                           \
        }                                                               \
        else                                                            \
        {                                                               \
            if(                                                         \
                freopen("/dev/null", "w", stderr) == NULL ||            \
                freopen("/dev/null", "w", stdout) == NULL               \
            )                                                           \
            {                                                           \
                printf("\n\n\tERROR REDIRECTING STD BUFFERS\n\n");      \
                exit(EXIT_SUCCESS);                                     \
            }                                                           \
            usleep(0);

#define TEST_REVERT_CLOSE       \
            exit(EXIT_SUCCESS); \
        }                       \
        __test_memory = false;  \
    }

#define ARG_OPEN(...) __VA_ARGS__

#endif

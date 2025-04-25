#ifndef __TEST_REVERT_H__
#define __TEST_REVERT_H__

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
        bool __is_main_process = true;

#define TEST_FN_CLOSE           \
        TEST_ASSERT_MEM_EMPTY   \
        if(!__is_main_process)  \
            exit(EXIT_SUCCESS); \
    }

#define TEST_LOG_ERROR(TAG, ...)                                                        \
    {                                                                                   \
        printf("\n\n\tERROR TEST\t| l:%d | %s %d | ", __LINE__, __func__, (int)(TAG));  \
        printf(__VA_ARGS__);                                                            \
        printf("\n\n");                                                                 \
        exit(EXIT_FAILURE);                                                             \
    }

#define TEST_CASE_OPEN(TAG)                                                 \
    if(__is_main_process)                                                   \
    {                                                                       \
        if(show)                                                            \
            printf("\n\t\t%s %2d\t\t", __func__, (int)(TAG));               \
        int pid_test = fork();                                              \
        assert(pid_test >= 0);                                              \
        if(pid_test < 0)                                                    \
        {                                                                   \
            TEST_LOG_ERROR(TAG, "FORKING TEST")                             \
        }                                                                   \
        if(pid_test)                                                        \
        {                                                                   \
            pid_t pid_timeout = fork();                                     \
            if(pid_timeout < 0)                                             \
            {                                                               \
                kill(pid_test, SIGKILL);                                    \
                TEST_LOG_ERROR(TAG, "FORKING TIMEOUT")                      \
            }                                                               \
            if(pid_timeout == 0)                                            \
            {                                                               \
                sleep(TEST_TIMEOUT);                                        \
                exit(EXIT_SUCCESS);                                         \
            }                                                               \
            int status;                                                     \
            pid_t pid_return = waitpid(0, &status, 0);                      \
            assert(pid_return > 0);                                         \
            if(pid_return == pid_timeout)                                   \
            {                                                               \
                kill(pid_test, SIGKILL);                                    \
                TEST_LOG_ERROR(TAG, "TEST TIMEOUT")                         \
            }                                                               \
            if(pid_return != pid_test)                                      \
            {                                                               \
                TEST_LOG_ERROR(TAG, "INVALID PID CAUGHT %d", pid_return)    \
            }                                                               \
            kill(pid_timeout, SIGKILL);                                     \
            waitpid(pid_timeout, NULL, 0);                                  \
            if(status != EXIT_SUCCESS)                                      \
            {                                                               \
                TEST_LOG_ERROR(TAG, "ERROR IN TEST EXECUTION ")             \
            }                                                               \
        }                                                                   \
        else                                                                \
        {                                                                   \
            __is_main_process = false;

#define TEST_CASE_CLOSE \
        }               \
    }

#define TEST_REVERT_OPEN                                            \
    {                                                               \
        int pid = fork();                                           \
        assert(pid >= 0);                                           \
        if(pid < 0)                                                 \
        {                                                           \
            printf("\n\n\tERROR FORKING");                          \
            exit(EXIT_FAILURE);                                     \
        }                                                           \
        if(pid)                                                     \
        {                                                           \
            int status;                                             \
            assert(waitpid(pid, &status, 0) >= 0);                  \
            if(status == EXIT_SUCCESS)                              \
            {                                                       \
                printf("\n\n\tTEST EXPECTED TO REVERT BUT DIDN'T"); \
                exit(EXIT_FAILURE);                                 \
            }                                                       \
        }                                                           \
        else                                                        \
        {                                                           \
            if(                                                     \
                freopen("/dev/null", "w", stdout) == NULL ||        \
                freopen("/dev/null", "w", stderr) == NULL           \
            )                                                       \
            {                                                       \
                exit(EXIT_SUCCESS);                                 \
            }                                                       \
            usleep(0);

#define TEST_REVERT_CLOSE       \
            exit(EXIT_SUCCESS); \
        }                       \
    }

#define ARG_OPEN(...) __VA_ARGS__

#endif

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

#define TEST_CASE_OPEN(TAG)                                                                     \
    if(__is_main_process)                                                                       \
    {                                                                                           \
        if(show)                                                                                \
            printf("\n\t\t%s %2d\t\t", __func__, (int)(TAG));                                   \
        int pid_test = fork();                                                                       \
        assert(pid_test >= 0);                                                                       \
        if(pid_test)                                                                                 \
        {                                                                                       \
            pid_t pid_timeout = fork();                 \
            if(pid_timeout < 0)                         \
            {                                           \
                kill(pid_test, SIGKILL);                \
                exit(EXIT_FAILURE);                     \
            }                                           \
            if(pid_timeout == 0)                        \
            {                                           \
                sleep(TIMEOUT);                         \
                exit(EXIT_SUCCESS);                     \
            }                                           \
            int status;                                 \
            pid_t pid_return = waitpid(0, &status, 0);  \
            assert(pid_return > 0);                     \
            if(pid_return == pid_timeout)               \
            {                                           \
                kill(pid_test, SIGKILL);                \
                printf("\n\n\tTest timeout\n\n");       \
                exit(EXIT_FAILURE);                     \
            }                                           \
            kill(pid_timeout, SIGKILL);                 \
            if(status != EXIT_SUCCESS)                                                          \
            {                                                                                   \
                printf("\n\n\tERROR TEST\t| l:%d | %s %d\t\t", __LINE__, __func__, (int)(TAG)); \
                exit(EXIT_FAILURE);                                                             \
            }                                                                                   \
        }                                                                                       \
        else                                                                                    \
        {                                                                                       \
            __is_main_process = false;

#define TEST_CASE_CLOSE \
        }               \
    }

#define TEST_REVERT_OPEN                                        \
    {                                                           \
        int pid = fork();                                       \
        assert(pid >= 0);                                       \
        if(pid)                                                 \
        {                                                       \
            int status;                                         \
            assert(waitpid(pid, &status, 0) >= 0);              \
            assert(status != EXIT_SUCCESS);                     \
        }                                                       \
        else                                                    \
        {                                                       \
            if(                                                 \
                freopen("/dev/null", "w", stdout) == NULL ||    \
                freopen("/dev/null", "w", stderr) == NULL       \
            )                                                   \
            {                                                   \
                exit(EXIT_SUCCESS);                             \
            }                                                   \
            usleep(0);

#define TEST_REVERT_CLOSE       \
            exit(EXIT_SUCCESS); \
        }                       \
    }

#define ARG_OPEN(...) __VA_ARGS__

#endif

#ifndef __TEST_REVERT_H__
#define __TEST_REVERT_H__

#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#include "./assert.h"

#define TEST_LIB printf("\n%s\t\t", __func__);

#define TEST_FN printf("\n\t%s\t\t", __func__);

#define TEST_CASE_OPEN(TAG)                                                                     \
    if(show) printf("\n\t\t%s %2d\t\t", __func__, (int)(TAG));                                  \
    {                                                                                           \
        int pid = fork();                                                                       \
        assert(pid >= 0);                                                                       \
        if(pid)                                                                                 \
        {                                                                                       \
            int status;                                                                         \
            assert(waitpid(pid, &status, 0) >= 0);                                              \
            if(status != EXIT_SUCCESS)                                                          \
            {                                                                                   \
                printf("\n\n\tERROR TEST\t| l:%d | %s %d\t\t", __LINE__, __func__, (int)(TAG)); \
                exit(EXIT_FAILURE);                                                             \
            }                                                                                   \
        }                                                                                       \
        else                                                                                    \
        {                                                                                       \
            usleep(0);

#define TEST_DEFAULT_CLOSE      \
            exit(EXIT_SUCCESS); \
        }                       \
    }

#define TEST_CASE_CLOSE     \
        TEST_ASSERT_EMPTY   \
    TEST_DEFAULT_CLOSE

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

#define TEST_REVERT_CLOSE TEST_DEFAULT_CLOSE

#define TEST_TIMEOUT_DEFAULT 5

#define TEST_TIMEOUT_OPEN(TIMEOUT)                      \
    {                                                   \
        pid_t pid_test = fork();                        \
        assert(pid_test >= 0);                          \
        if(pid_test)                                    \
        {                                               \
            pid_t pid_timeout = fork();                 \
            assert(pid_timeout >= 0);                   \
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
            if(status)                                  \
            {                                           \
                printf("\n\n\tTest faillure\n\n");      \
                exit(EXIT_FAILURE);                     \
            }                                           \
        }                                               \
        else                                            \
        {

#define TEST_TIMEOUT_OPEN_DEFAULT TEST_TIMEOUT_OPEN(TEST_TIMEOUT_DEFAULT)

#define TEST_TIMEOUT_CLOSE TEST_DEFAULT_CLOSE

#define ARG_OPEN(...) __VA_ARGS__

#endif

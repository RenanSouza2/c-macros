#ifndef MACROS_TEST_H
#define MACROS_TEST_H

#include <signal.h>
#include <stdarg.h>
#include <time.h>

#include "assert.h"
#include "fork.h"
#include "uint.h"

constexpr uint64_t TEST_MS_PER_SEC = 1000;
constexpr uint64_t TEST_NS_PER_MS = 1000000;
constexpr uint64_t TEST_FUZZ_TAG_MULTIPLIER = 1000000;

// TODO TIMEOUT LIB
#define TEST_LIB printf("\n%s\t\t", __func__);

#define FUNC_TAG __func__

#define TEST_FN_OPEN                    \
    {                                   \
        printf("\n\t%s\t\t", FUNC_TAG); \
        bool _is_main_process = true;   \
        bool _test_memory = true;       \
        uint64_t _tag = 0;

#define TEST_FN_CLOSE               \
        if(_test_memory)            \
        {                           \
            TEST_ASSERT_MEM_EMPTY   \
        }                           \
        if(!_is_main_process)       \
        {                           \
            exit(EXIT_SUCCESS);     \
        }                           \
    }

[[noreturn, gnu::format(printf, 4, 5)]]
static void test_log_error(
    uint64_t _tag,
    const char func[],
    uint64_t line,
    const char format[],
    ...
)
{
    va_list args;
    va_start(args, format);
    printf("\n\n\tERROR TEST\t| f: %s | l: " U64P() " | tag: " U64P() " | ", func, line, _tag);
    vprintf(format, args);
    printf("\n\n");
    assert(false);
}

// returns true if main process
[[nodiscard, maybe_unused]]
static bool start_case(
    uint64_t _tag,
    uint64_t line,
    const char func[],
    bool show,
    uint64_t timeout_ms
)
{
    if(show)
    {
        printf("\n\t\t%s " U64P(2) "\t\t", func, _tag);
    }

    pid_t pid = fork_safe();
    if(pid)
    {
        int status;
        waitpid_safe(pid, &status);
        assert(status == EXIT_SUCCESS);
        return true;
    }

    pid_t pid_test = fork_safe();
    if(pid_test == 0)
    {
        return false;
    }

    int status;
    if(timeout_ms)
    {
        pid_t pid_timeout = fork_safe();
        if(pid_timeout == 0)
        {
            struct timespec spec = (struct timespec)
            {
                .tv_sec = (long)(timeout_ms / TEST_MS_PER_SEC),
                .tv_nsec = (long)((timeout_ms % TEST_MS_PER_SEC) * TEST_NS_PER_MS)
            };
            nanosleep(&spec, nullptr);
            exit(EXIT_SUCCESS);
        }

        pid_t pid_return = waitpid_safe(0, &status);
        if(pid_return == pid_timeout)
        {
            kill(pid_test, SIGKILL);
            test_log_error(_tag, func, line, "TEST TIMEOUT");
        }

        if(pid_return != pid_test)
        {
            test_log_error(_tag, func, line, "INVALID PID CAUGHT %d", pid_return);
        }

        kill(pid_timeout, SIGKILL);
    }
    else
    {
        waitpid_safe(pid_test, &status);
    }

    if(status != EXIT_SUCCESS)
    {
        test_log_error(_tag, func, line, "ERROR IN TEST EXECUTION ");
    }
    exit(EXIT_SUCCESS);
}



#define TEST_CASE_OPEN_TIMEOUT(TAG, TIMEOUT)                                    \
    if(_is_main_process)                                                        \
    {                                                                           \
        _tag = (uint64_t)(TAG);                                                 \
        _is_main_process = start_case(_tag, __LINE__, FUNC_TAG, show, TIMEOUT); \
        if(!_is_main_process)                                                   \
        {

#define TEST_CASE_OPEN(TAG) TEST_CASE_OPEN_TIMEOUT(TAG, TEST_CASE_TIMEOUT_MS)

#define TEST_CASE_CLOSE \
        }               \
    }

[[maybe_unused]]
static uint64_t tag_fuzz_get(uint64_t _tag, uint64_t _i)
{
    return (_tag * TEST_FUZZ_TAG_MULTIPLIER) + _i;
}

#define TEST_FUZZ_CASE_OPEN(TAG, RUNS)      \
    {                                       \
        for(uint64_t _i=0; _i<(RUNS); _i++) \
        {                                   \
            TEST_CASE_OPEN_TIMEOUT(tag_fuzz_get(TAG, _i), 0)    \
            {

#define TEST_FUZZ_CASE_CLOSE    \
            }                   \
            TEST_CASE_CLOSE     \
        }                       \
    }

[[maybe_unused]]
static pid_t start_revert(uint64_t _tag, uint64_t line, const char func[])
{
    pid_t pid = fork();
    if(pid < 0)
    {
        test_log_error(_tag, func, line, "ERROR FORKING");
    }
    if(pid)
    {
        int status;
        waitpid_safe(pid, &status);
        if(status == EXIT_SUCCESS)
        {
            test_log_error(_tag, func, line, "TEST EXPECTED TO REVERT BUT DIDN'T");
        }
    }
    else
    {
        if(
            freopen("/dev/null", "w", stderr) == nullptr ||
            freopen("/dev/null", "w", stdout) == nullptr
        )
        {
            printf("\n\n\tERROR REDIRECTING STD BUFFERS\n\n");
            exit(EXIT_SUCCESS);
        }
        struct timespec spec = {};
        nanosleep(&spec, nullptr);
    }
    return pid;
}

#define TEST_REVERT_OPEN                                \
    {                                                   \
        if(start_revert(_tag, __LINE__, __func__) == 0) \
        {

#define TEST_REVERT_CLOSE       \
            exit(EXIT_SUCCESS); \
        }                       \
        _test_memory = false;   \
    }

#define ARG_OPEN(...) __VA_ARGS__

#endif

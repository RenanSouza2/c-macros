#ifndef MACROS_ASSERT_H
#define MACROS_ASSERT_H

#include <stdarg.h>
#include <stdio.h> // IWYU pragma: keep
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>



#undef  assert



#ifdef DEBUG

#define TRIGGER_SANITIZER *((int*)0xDEAD) = 1;

#else  // DEBUG

#define TRIGGER_SANITIZER

#endif // DEBUG

// One write per record: up to 512 bytes, _POSIX_PIPE_BUF, a pipe shared with
// other processes takes it whole.
[[gnu::format(printf, 1, 2)]]
static inline void tprintf_write(const char *fmt, ...)
{
    char record[512];
    va_list args;
    va_start(args, fmt);
    int len = vsnprintf(record, sizeof(record), fmt, args);
    va_end(args);
    if(len < 0)
    {
        return;
    }

    size_t size = (size_t)len;
    if(size >= sizeof(record))
    {
        // cut to fit, keeping the closing tab
        size = sizeof(record) - 1;
        record[size - 1] = '\t';
    }
    ssize_t res = write(STDERR_FILENO, record, size);
    (void)res;
}

#define tprintf(FMT, ...) \
    tprintf_write("\n%-16s| " FMT "\t", __func__ __VA_OPT__(,) __VA_ARGS__)

#if defined(DEBUG) || defined(ASSERT_VERBOSE)

#define TRAP(MSG)                                                               \
    {                                                                           \
        fprintf(stderr, "\n\n");                                                \
        fprintf(stderr, "%s:%d: %s: %s\n", __FILE__, __LINE__, __func__, MSG);  \
        fprintf(stderr, "\n");                                                  \
        TRIGGER_SANITIZER                                                       \
        exit(EXIT_FAILURE);                                                     \
    }

#else

#define TRAP(MSG) exit(EXIT_FAILURE);

#endif // DEBUG || ASSERT_VERBOSE

#define assert(COND)                                \
    {                                               \
        if(!(COND))                                 \
        {                                           \
            TRAP("Assertion '" #COND "' failed")    \
        }                                           \
    }

#define revert() TRAP("Reached unreachable code")

#endif // MACROS_ASSERT_H

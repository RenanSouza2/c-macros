#ifndef MACROS_ASSERT_H
#define MACROS_ASSERT_H

#include <stdio.h> // IWYU pragma: keep
#include <stdlib.h>
#include <assert.h>



#undef  assert



#ifdef DEBUG

#define TRIGGER_SANITIZER *((int*)0xDEAD) = 1;

#else  // DEBUG

#define TRIGGER_SANITIZER

#endif // DEBUG

// single fprintf call so the whole line reaches the fd in one write(),
// keeping lines from interwoven processes from splitting mid-line
#define tprintf(FMT, ...) \
    fprintf(stderr, "\n%s\t| " FMT "\t", __func__ __VA_OPT__(,) __VA_ARGS__)

#if defined(DEBUG) || defined(ASSERT_VERBOSE)

#define assert(COND)                                                                                    \
    {                                                                                                   \
        if(!(COND))                                                                                     \
        {                                                                                               \
            fprintf(stderr, "\n\n");                                                                    \
            fprintf(stderr, "%s:%d: %s: Assertion '%s' failed\n", __FILE__, __LINE__, __func__, #COND); \
            fprintf(stderr, "\n");                                                                      \
            TRIGGER_SANITIZER                                                                           \
            exit(EXIT_FAILURE);                                                                         \
        }                                                                                               \
    }


#else

#define assert(COND)            \
    {                           \
        if(!(COND))             \
        {                       \
            exit(EXIT_FAILURE); \
        }                       \
    }

#endif // DEBUG || ASSERT_VERBOSE

#endif // MACROS_ASSERT_H

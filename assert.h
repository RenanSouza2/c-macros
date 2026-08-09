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

#define tprintf(FMT, ...) \
    fprintf(stderr, "\n%-16s| " FMT "\t", __func__ __VA_OPT__(,) __VA_ARGS__)

#if defined(DEBUG) || defined(ASSERT_VERBOSE)

#define TRAP(MSG)                                                               \
    {                                                                           \
        fprintf(stderr, "\n\n");                                                \
        fprintf(stderr, "%s:%d: %s: " MSG "\n", __FILE__, __LINE__, __func__);  \
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

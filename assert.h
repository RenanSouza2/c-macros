#ifndef __ASSERT_H__
#define __ASSERT_H__

#include <stdio.h>
#include <assert.h>

#undef  assert

#ifdef DEBUG

#define assert(COND)                \
    if(!(COND))                     \
    {                               \
        fprintf(stderr, "\n\n");    \
        fprintf(stderr, "%s:%d: %s: Assertion '%s' failled", __FILE__, __LINE__, __ASSERT_FUNCTION, #COND);    \
        fprintf(stderr, "\n");      \
        *((int*)0xDEAD) = 1;        \
        exit(EXIT_FAILURE);         \
    }

#define assert_dbg(COND) assert(COND)

#define dprintf(...)                    \
    {                                   \
        printf("\n%s\t| ", __func__);   \
        printf(__VA_ARGS__);            \
        printf("\t");                   \
    }

#else

#define assert(COND)        \
    if(!(COND))                     \
    {                               \
        fprintf(stderr, "\n\n");    \
        fprintf(stderr, "%s:%d: %s: Assertion '%s' failled", __FILE__, __LINE__, __ASSERT_FUNCTION, #COND);    \
        fprintf(stderr, "\n");      \
        exit(EXIT_FAILURE);         \
    }

#define assert_dbg(COND)

#endif // DEBUG

#endif // __ASSERT_H__

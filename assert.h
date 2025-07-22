#ifndef __ASSERT_H__
#define __ASSERT_H__

#include <stdio.h>
#include <assert.h>

#undef  assert

#ifdef NDEBUG

#define assert(COND)        \
    if(!(COND))             \
        exit(EXIT_FAILURE); \

#elif defined __linux__

#define assert(COND)                                                    \
    if(!(COND))                                                         \
    {                                                                   \
        fprintf(stderr, "\n\n");                                        \
        __assert_fail(#COND, __FILE__, __LINE__, __ASSERT_FUNCTION);    \
    }

#elif defined __APPLE__

#define assert(COND)                                                    \
    if(!(COND))                                                         \
    {                                                                   \
        fprintf(stderr, "\n\n");                                        \
        __assert_rtn(__func__, __ASSERT_FILE_NAME, __LINE__, #COND);    \
    }

#endif // __APPLE__

#ifdef DEBUG

#define assert_dbg(COND) assert(COND)

#else

#define assert_dbg(COND)

#endif // DEBUG

#endif // __ASSERT_H__

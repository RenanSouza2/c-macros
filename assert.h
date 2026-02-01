#ifndef __ASSERT_H__
#define __ASSERT_H__

#include <stdio.h>
#include <assert.h>



#undef  assert



#ifdef DEBUG

#define TRIGGER_SANITIZER *((int*)0xDEAD) = 1;

#else  // DEBUG

#define TRIGGER_SANITIZER

#endif // DEBUG



#ifdef __linux__

#define ASSERT_PRINT(COND) fprintf(stderr, "%s:%d: %s: Assertion '%s' failled", __FILE__, __LINE__, __ASSERT_FUNCTION, COND);

#elif defined __APPLE__

#define ASSERT_PRINT(COND) fprintf(stderr, "%s:%d: %s: Assertion '%s' failed\n", __FILE__, __LINE__, __func__, COND);

#endif



#define assert(COND)                \
    if(!(COND))                     \
    {                               \
        fprintf(stderr, "\n\n");    \
        ASSERT_PRINT(#COND)         \
        fprintf(stderr, "\n");      \
        TRIGGER_SANITIZER           \
        exit(EXIT_FAILURE);         \
    }



#define tprintf(...)                            \
    {                                           \
        fprintf(stderr, "\n%s\t| ", __func__);  \
        fprintf(stderr, __VA_ARGS__);           \
        fprintf(stderr, "\t");                  \
    }



#endif // __ASSERT_H__

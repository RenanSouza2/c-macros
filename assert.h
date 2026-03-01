#ifndef __ASSERT_H__
#define __ASSERT_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>



#undef  assert



#ifdef DEBUG

#define TRIGGER_SANITIZER *((int*)0xDEAD) = 1;

#else  // DEBUG

#define TRIGGER_SANITIZER

#endif // DEBUG



#ifdef __linux__

#define __FN __ASSERT_FUNCTION

#elif defined __APPLE__

#define __FN __func__

#endif



#define assert(COND)                \
    if(!(COND))                     \
    {                               \
        fprintf(stderr, "\n\n");    \
        fprintf(stderr, "%s:%d: %s: Assertion '%s' failed\n", __FILE__, __LINE__, __FN, #COND);  \
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

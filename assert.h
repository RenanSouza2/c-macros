#ifndef __ASSERT_H__
#define __ASSERT_H__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>



#undef  assert



#ifdef DEBUG

#define TRIGGER_SANITIZER *((int*)0xDEAD) = 1;

#else  // DEBUG

#define TRIGGER_SANITIZER

#endif // DEBUG



#define assert(COND)                \
    if(!(COND))                     \
    {                               \
        fprintf(stderr, "\n\n");    \
        fprintf(stderr, "%s:%d: %s: Assertion '%s' failed\n", __FILE__, __LINE__, __func__, #COND); \
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

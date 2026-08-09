#ifndef MACROS_THREADS_H
#define MACROS_THREADS_H

#include <pthread.h>

#include "assert.h"

#define TREAT(CALL) assert((CALL) == 0);

#endif // MACROS_THREADS_H

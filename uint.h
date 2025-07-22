#ifndef __U64_H__
#define __U64_H__

#include <stdint.h>

#define U64(VALUE) ((uint64_t)(VALUE))

typedef uint64_t * uint64_p;
typedef __uint128_t uint128_t;
typedef __iint128_t iint128_t;

#ifdef __linux__

#define D64P(C) "%" #C "ld"
#define U64P(C) "%" #C "lu"
#define U64PX "%016lx"

#elif defined __APPLE__

#define D64P(C) "%" #C "lld"
#define U64P(C) "%" #C "llu"
#define U64PX "%016llx"

#endif

#endif

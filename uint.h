#ifndef __U64_H__
#define __U64_H__

#include <stdint.h>

#define U64(VALUE) ((uint64_t)(VALUE))
#define U128(V) ((uint128_t)(V))
#define U128_IMMED(V1, V2) ((U128(V1) << 64) | (V2))
#define MUL(V1, V2) U128(V1) * U128(V2)
#define LOW(V) U64(V)
#define HIGH(V) U64((V) >> 64)
#define B(BITS) (1UL << (BITS))

typedef uint64_t * uint64_p;
typedef __uint128_t uint128_t;
typedef __int128_t int128_t;

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

#ifndef __U64_H__
#define __U64_H__

#include <stdint.h>
#include <inttypes.h>

typedef uint64_t * uint64_p;
typedef __uint128_t uint128_t;
typedef __int128_t int128_t;

#define U64(VALUE) ((uint64_t)(VALUE))
#define U128(V) ((uint128_t)(V))
#define I128(V) ((int128_t)(V))
#define U128HL(V1, V2) ((U128(V1) << 64) | (V2))
#define MUL(V1, V2) U128(V1) * U128(V2)
#define LOW(V) U64(V)
#define HIGH(V) U64((V) >> 64)
#define B(BITS) (1UL << (BITS))
#define B128(BITS) (U128(1UL) << (BITS))

#define UINT128_MAX U128HL(UINT64_MAX, UINT64_MAX)
#define INT128_MAX I128(U128HL(UINT64_MAX / 2, UINT64_MAX))
#define INT128_MIN I128(U128HL(B(63), 0))

#define D64P(C) "%" #C PRIi64
#define U64P(C) "%" #C PRIu64
#define U64PX "%016" PRIx64

#define U128PX U64P U64P
#define U128A(C) HIGH(C), LOW(C)

#endif

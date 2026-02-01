#ifndef CLU_STDBIT_FIX_H
#define CLU_STDBIT_FIX_H

// 1. If the system actually has the file (Linux/Future Mac), use it.
#if defined(__has_include) && __has_include(<stdbit.h>)
    #include <stdbit.h>

// 2. Otherwise, map the C23 standard names to Clang builtins
#else // has stdbit.h
    #include <stdint.h>

    // Count Leading Zeros (clz)
    #define stdc_trailing_zeros(x)                  \
        ((unsigned int)_Generic((x),                \
            unsigned int: __builtin_ctz(x),         \
            unsigned long: __builtin_ctzl(x),       \
            unsigned long long: __builtin_ctzll(x), \
            default: __builtin_ctz(x)))

    // Count Trailing Zeros (ctz)
    #define stdc_leading_zeros(x) \
        ((unsigned int)_Generic((x), \
            unsigned int: __builtin_clz(x), \
            unsigned long: __builtin_clzl(x), \
            unsigned long long: __builtin_clzll(x), \
            default: __builtin_clz(x)))

    // Count Ones (popcount)
    #define stdc_count_ones(x) \
        ((unsigned int)_Generic((x), \
            unsigned int: __builtin_popcount(x), \
            unsigned long: __builtin_popcountl(x), \
            unsigned long long: __builtin_popcountll(x), \
            default: __builtin_popcount(x)))

    // Bit Width (log2)
    #define stdc_bit_width(x) \
        ((x) == 0 ? 0 : (sizeof(x) * 8 - stdc_leading_zeros(x)))

    #define stdc_bit_ceil(x) \
        ((x) <= 1 ? 1 : ((uint64_t)1 << stdc_bit_width((x) - 1)))

#endif // has stdbit.h

#endif

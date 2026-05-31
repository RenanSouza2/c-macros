#ifndef __TIME_H__
#define __TIME_H__

#include <time.h>

#include "uint.h" // IWYU pragma: keep

constexpr uint64_t nanosecs_in_sec = 1'000'000'000;

#define TIME_RESET _time_begin = get_time();

#define TIME_SETUP          \
    uint64_t _time_begin;   \
    uint64_t _time_end;     \
    TIME_RESET

#define TIME_END(TIME_VAR)                          \
    _time_end = get_time();                         \
    uint64_t TIME_VAR = _time_end - _time_begin;    \
    _time_begin = _time_end;

#ifdef __linux__
    #define CLU_CLOCK_ID CLOCK_MONOTONIC_RAW
#elifdef __APPLE__
    #define CLU_CLOCK_ID CLOCK_MONOTONIC
#endif

[[nodiscard, maybe_unused]]
static uint64_t get_time()
{
    struct timespec time;
    clock_gettime(CLU_CLOCK_ID, &time);
<<<<<<< HEAD
    return ((uint64_t)time.tv_sec * nanosecs_in_sec) + (uint64_t)time.tv_nsec;
=======
    constexpr uint64_t nanoseconds_per_second = 1'000'000'000;
    return ((uint64_t)time.tv_sec * nanoseconds_per_second) + (uint64_t)time.tv_nsec;
>>>>>>> origin/v3.3
}

[[nodiscard, maybe_unused]]
static double dtime(uint64_t t)
{
    constexpr uint64_t nanoseconds_per_second = 1'000'000'000;
    return (double)t / nanoseconds_per_second;
}

#endif

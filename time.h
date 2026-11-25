#ifndef MACROS_TIME_H
#define MACROS_TIME_H

#include <time.h>

#include "uint.h" // IWYU pragma: keep

constexpr uint64_t nanosecs_in_sec = 1'000'000'000;

#define TIME_RESET _time_begin = get_time();

#define TIME_SETUP                      \
    uint64_t _time_begin, _time_end;    \
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
    return ((uint64_t)time.tv_sec * nanosecs_in_sec) + (uint64_t)time.tv_nsec;
}

[[nodiscard, maybe_unused]]
static double dtime(uint64_t t)
{
    return (double)t / nanosecs_in_sec;
}

// Wall-clock (CLOCK_REALTIME, i.e. Unix epoch) time as a double, for log
// lines a consumer needs to compare against its own epoch clock (e.g. a
// dashboard replaying a log file after a restart, where get_time()'s
// CLOCK_MONOTONIC[_RAW] value -- meant for in-process duration math via
// TIME_SETUP/TIME_END -- has no meaningful cross-process reference point).
// CLOCK_REALTIME is POSIX-standard on both Linux and macOS, so no ifdef is
// needed here the way CLU_CLOCK_ID needs one above.
[[nodiscard, maybe_unused]]
static double get_wall_time()
{
    struct timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    return (double)time.tv_sec + ((double)time.tv_nsec / (double)nanosecs_in_sec);
}

#endif

#ifndef __TIME_H__
#define __TIME_H__

#include <time.h>

#include "uint.h"

#define TIME_RESET _time_begin = get_time();

#define TIME_SETUP                      \
    uint64_t _time_begin, _time_end;    \
    TIME_RESET

#define TIME_END(TIME_VAR)                          \
    _time_end = get_time();                         \
    uint64_t TIME_VAR = _time_end - _time_begin;    \
    _time_begin = _time_end;

#if defined(__linux__)
    #define CLU_CLOCK_ID CLOCK_MONOTONIC_RAW
#elif defined(__APPLE__)
    #define CLU_CLOCK_ID CLOCK_MONOTONIC
#endif

static uint64_t get_time(void)
{
    struct timespec time;
    clock_gettime(CLU_CLOCK_ID, &time);
    return (uint64_t)time.tv_sec * 1000000000ULL + (uint64_t)time.tv_nsec;
}

#endif

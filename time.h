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

static uint64_t get_time()
{
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC_RAW, &time);
    return time.tv_sec * (uint64_t)1e9 + time.tv_nsec;
}

#endif
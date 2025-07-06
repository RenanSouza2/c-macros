#ifndef __TIME_H__
#define __TIME_H__

#include <time.h>

#include "U64.h"

uint64_t get_time();

#define TIME_SETUP uint64_t _time_begin, _time_end;

#define TIME_START _time_begin = get_time();

#define TIME_END(TIME_VAR)                          \
    _time_end = get_time();                         \
    uint64_t TIME_VAR = _time_end - _time_begin;    \

#ifndef SKIP_TIME

uint64_t get_time()
{
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC_RAW, &time);
    return time.tv_sec * (uint64_t)1e9 + time.tv_nsec;
}

#endif

#endif
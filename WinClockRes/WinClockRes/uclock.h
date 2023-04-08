// Get the current time from a POSIX clockid_t. Windows version.
//
// Copyright (c) 2019 Jorge Ramos (mailto jramos at pobox dot com)
//
// This is free software. Redistribution and use in source and binary forms,
// with or without modification, for any purpose and with or without fee are
// hereby permitted. Altered source versions must be plainly marked as such.
//
// If you find this software useful, an acknowledgment would be appreciated
// but is not required.
//
// THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT ANY WARRANTY OR CONDITION.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE TO ANYONE
// FOR ANY DAMAGES RELATED TO THIS SOFTWARE, UNDER ANY KIND OF LEGAL CLAIM.

#ifndef _UCLOCK_H_AEE9E75E_B3DB_4D50_AE80_992FEDC8FBE8_
#define _UCLOCK_H_AEE9E75E_B3DB_4D50_AE80_992FEDC8FBE8_

#ifdef _MSC_VER
#pragma once
#endif

#include <time.h>

#define CLOCK_REALTIME                  0       // System-wide realtime clock.
#define CLOCK_MONOTONIC                 1       // Clock that cannot be set and represents monotonic time since some unspecified starting point.
#define CLOCK_PROCESS_CPUTIME_ID        2       // High-resolution per-process timer from the CPU.
#define CLOCK_THREAD_CPUTIME_ID         3       // Thread-specific CPU-time clock.
#define CLOCK_MONOTONIC_RAW             4
#define CLOCK_REALTIME_COARSE           5
#define CLOCK_MONOTONIC_COARSE          6

typedef int clockid_t;

#ifdef __cplusplus
extern "C" {
#endif

    int clock_getres(clockid_t clk_id, struct timespec* res);
    int clock_gettime(clockid_t clk_id, struct timespec* tp);
    int clock_settime(clockid_t clk_id, const struct timespec* tp);

    void clock_test1();

#ifdef __cplusplus
}
#endif

#endif

/*
 * Copyright (C) 2013 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#pragma once

#include <errno.h>
#include <time.h>
#include <sys/cdefs.h>

#include "private/bionic_constants.h"

bool timespec_from_timeval(timespec& ts, const timeval& tv);
void timespec_from_ms(timespec& ts, const int ms);

void timeval_from_timespec(timeval& tv, const timespec& ts);

void monotonic_time_from_realtime_time(timespec& monotonic_time, const timespec& realtime_time);
void realtime_time_from_monotonic_time(timespec& realtime_time, const timespec& monotonic_time);

static inline int64_t to_ns(const timespec& ts) {
  return ts.tv_sec * NS_PER_S + ts.tv_nsec;
}

static inline int64_t to_us(const timeval& tv) {
  return tv.tv_sec * US_PER_S + tv.tv_usec;
}

static inline int check_timespec(const timespec* ts, bool null_allowed) {
  if (null_allowed && ts == nullptr) {
    return 0;
  }
  // glibc just segfaults if you pass a null timespec.
  // That seems a lot more likely to catch bad code than returning EINVAL.
  if (ts->tv_nsec < 0 || ts->tv_nsec >= NS_PER_S) {
    return EINVAL;
  }
  if (ts->tv_sec < 0) {
    return ETIMEDOUT;
  }
  return 0;
}

#if !defined(__LP64__)
static inline void absolute_timespec_from_timespec(timespec& abs_ts, const timespec& ts, clockid_t clock) {
  clock_gettime(clock, &abs_ts);
  abs_ts.tv_sec += ts.tv_sec;
  abs_ts.tv_nsec += ts.tv_nsec;
  if (abs_ts.tv_nsec >= NS_PER_S) {
    abs_ts.tv_nsec -= NS_PER_S;
    abs_ts.tv_sec++;
  }
}
#endif

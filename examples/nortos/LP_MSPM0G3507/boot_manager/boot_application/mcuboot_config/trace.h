/*
 * Copyright (c) 2015-2017 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * \file trace.h
 * Trace interface abstraction for NanoStack library as well as application.
 *
 * Actual used trace library is mbed-trace. For usage details check mbed_trace.h.
 *
 */
#ifndef TRACE_H_
#define TRACE_H_

#if defined(HAVE_DEBUG) && !defined(FEA_TRACE_SUPPORT)
#define FEA_TRACE_SUPPORT
#endif

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

/** this print is some deep information for debug purpose */
#define TRACE_LEVEL_DEBUG 0x10
/** Info print, for general purpose prints */
#define TRACE_LEVEL_INFO 0x08
/** warning prints, which shouldn't causes any huge problems */
#define TRACE_LEVEL_WARN 0x04
/** Error prints, which causes probably problems, e.g. out of mem. */
#define TRACE_LEVEL_ERROR 0x02

#ifndef TRACE_LOG_LEVEL
#define TRACE_LOG_LEVEL TRACE_LEVEL_DEBUG
#endif

#ifndef DeviceFamily_CC23X0
#define TRACE_MAX_LEVEL TRACE_LOG_LEVEL
#else
#define TRACE_MAX_LEVEL 0
#endif

#if TRACE_MAX_LEVEL >= TRACE_LEVEL_DEBUG
#define TRACE_LOG_DBG(...)                       \
    trace_printf(TRACE_LEVEL_DEBUG, TRACE_GROUP, \
        __VA_ARGS__)  //!< Print debug message
#else
#define TRACE_LOG_DBG(...)
#endif

#if TRACE_MAX_LEVEL >= TRACE_LEVEL_INFO
#define TRACE_LOG_INF(...) \
    trace_printf(          \
        TRACE_LEVEL_INFO, TRACE_GROUP, __VA_ARGS__)  //!< Print info message
#else
#define TRACE_LOG_INF(...)
#endif

#if TRACE_MAX_LEVEL >= TRACE_LEVEL_WARN
#define TRACE_LOG_WRN(...)                      \
    trace_printf(TRACE_LEVEL_WARN, TRACE_GROUP, \
        __VA_ARGS__)  //!< Print warning message
#else
#define TRACE_LOG_WRN(...)
#endif

#if TRACE_MAX_LEVEL >= TRACE_LEVEL_ERROR
#define TRACE_LOG_ERR(...)                       \
    trace_printf(TRACE_LEVEL_ERROR, TRACE_GROUP, \
        __VA_ARGS__)  //!< Print Error Message
#else
#define TRACE_LOG_ERR(...)
#endif

#ifdef EXCLUDE_TRACE

#define trace_init()
#define trace_printf(dlevel, grp, fmt, ...)

#else
//aliases for the most commonly used functions and the helper functions
void trace_printf(uint8_t dlevel, const char *grp, const char *fmt, ...);
void trace_vprintf(
    uint8_t dlevel, const char *grp, const char *fmt, va_list ap);
void trace_init(void);
void trace_enable_module();
void trace_disable_module(void);
void trace_put_char_blocking(const char ch);
void trace_enable_exception_trace(void);
bool trace_enable_data_trace(const uint32_t *variable);
void trace_enable_cycle_counter(void);
void trace_flush_module(void);
char *trace_array(const uint8_t *buf, uint16_t len);
#endif /* EXCLUDE_TRACE */

#endif /* TRACE_H_ */

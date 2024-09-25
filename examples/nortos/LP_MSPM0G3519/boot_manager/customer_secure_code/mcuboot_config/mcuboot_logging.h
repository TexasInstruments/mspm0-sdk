/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 * Copyright (c) 2015 Runtime Inc
 * Copyright (c) 2020 Cypress Semiconductor Corporation
 * Copyright (c) 2021 Texas Instruments
 *
 * SPDX-License-Identifier: Apache-2.0
 */
/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
/*******************************************************************************/

#ifndef MCUBOOT_LOGGING_H
#define MCUBOOT_LOGGING_H

#include "trace.h"

#ifdef EXCLUDE_TRACE
#define MCUBOOT_LOG_ERR(_fmt, ...)
#define MCUBOOT_LOG_WRN(_fmt, ...)
#define MCUBOOT_LOG_INF(_fmt, ...)
#define MCUBOOT_LOG_DBG(_fmt, ...)
#define MCUBOOT_LOG_MODULE_DECLARE(...)
#define MCUBOOT_LOG_MODULE_REGISTER(...)

#else

#define TRACE_GROUP "MCB"

#ifndef MCUBOOT_LOG_LEVEL
#define MCUBOOT_LOG_LEVEL TRACE_LEVEL_DEBUG
#endif

#ifndef TRACE_MAX_LEVEL
#define TRACE_MAX_LEVEL MCUBOOT_LOG_LEVEL
#endif

#if TRACE_MAX_LEVEL >= TRACE_LEVEL_DEBUG
#define MCUBOOT_LOG_DBG(...)                     \
    trace_printf(TRACE_LEVEL_DEBUG, TRACE_GROUP, \
        __VA_ARGS__)  //!< Print debug message
#else
#define MCUBOOT_LOG_DBG(...)
#endif

#if TRACE_MAX_LEVEL >= TRACE_LEVEL_INFO
#define MCUBOOT_LOG_INF(...) \
    trace_printf(            \
        TRACE_LEVEL_INFO, TRACE_GROUP, __VA_ARGS__)  //!< Print info message
#else
#define MCUBOOT_LOG_INF(...)
#endif

#if TRACE_MAX_LEVEL >= TRACE_LEVEL_WARN
#define MCUBOOT_LOG_WRN(...)                    \
    trace_printf(TRACE_LEVEL_WARN, TRACE_GROUP, \
        __VA_ARGS__)  //!< Print warning message
#else
#define MCUBOOT_LOG_WRN(...)
#endif

#if TRACE_MAX_LEVEL >= TRACE_LEVEL_ERROR
#define MCUBOOT_LOG_ERR(...)                     \
    trace_printf(TRACE_LEVEL_ERROR, TRACE_GROUP, \
        __VA_ARGS__)  //!< Print Error Message
#else
#define MCUBOOT_LOG_ERR(...)
#endif

#define MCUBOOT_LOG_MODULE_DECLARE(...)
#define MCUBOOT_LOG_MODULE_REGISTER(...)

#endif  //EXCLUDE_TRACE

#endif  //MCUBOOT_LOGGING_H

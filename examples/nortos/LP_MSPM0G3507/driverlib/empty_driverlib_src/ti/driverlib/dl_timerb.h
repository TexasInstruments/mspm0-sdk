/*
 * Copyright (c) 2025, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*!****************************************************************************
 *  @file       dl_timerb.h
 *  @brief      Basic Timer (BTIM) Driver Library
 *  @defgroup   BTIM Basic Timer (BTIM)
 *
 *  @anchor ti_dl_dl_timerb__Overview
 *  # Overview
 *  The Basic Timer (BTIM) Driver Library allows full configuration of the
 *  MSPM0 BTIM module. The BTIM module is an array of counters which can
 *  generate periodic interrupts, be concatenated to create longer time
 *  durations, and measure timing properties of external events.
 *
 *  <hr>
 ******************************************************************************
 */
/** @addtogroup BTIM
 * @{
 */
#ifndef ti_dl_dl_timerb__include
#define ti_dl_dl_timerb__include

#include <stdbool.h>
#include <stdint.h>

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/dl_common.h>

#ifdef __MSPM0_HAS_TIMER_B__

#ifdef __cplusplus
extern "C" {
#endif

/* clang-format off */

/** @addtogroup DL_TIMERB_INTERRUPT
 *  @{
 */

/*!
 * @brief Counter 0 Overflow interrupt
 */
#define DL_TIMERB_INTERRUPT_CNT0_OVF_EVENT   (BTIMER_CPU_INT_IMASK_CNT0OVF_SET)

/*!
 * @brief Counter 0 Start interrupt
 */
#define DL_TIMERB_INTERRUPT_CNT0_START_EVENT (BTIMER_CPU_INT_IMASK_CNT0STRT_SET)

/*!
 * @brief Counter 0 Stop interrupt
 */
#define DL_TIMERB_INTERRUPT_CNT0_STOP_EVENT (BTIMER_CPU_INT_IMASK_CNT0STOP_SET)

/*!
 * @brief Counter 1 Overflow interrupt
 */
#define DL_TIMERB_INTERRUPT_CNT1_OVF_EVENT   (BTIMER_CPU_INT_IMASK_CNT1OVF_SET)

/*!
 * @brief Counter 1 Start interrupt
 */
#define DL_TIMERB_INTERRUPT_CNT1_START_EVENT (BTIMER_CPU_INT_IMASK_CNT1STRT_SET)

/*!
 * @brief Counter 1 Stop interrupt
 */
#define DL_TIMERB_INTERRUPT_CNT1_STOP_EVENT (BTIMER_CPU_INT_IMASK_CNT1STOP_SET)

/*!
 * @brief Counter 2 Overflow interrupt
 */
#define DL_TIMERB_INTERRUPT_CNT2_OVF_EVENT   (BTIMER_CPU_INT_IMASK_CNT2OVF_SET)

/*!
 * @brief Counter 2 Start interrupt
 */
#define DL_TIMERB_INTERRUPT_CNT2_START_EVENT (BTIMER_CPU_INT_IMASK_CNT2STRT_SET)

/*!
 * @brief Counter 2 Stop interrupt
 */
#define DL_TIMERB_INTERRUPT_CNT2_STOP_EVENT (BTIMER_CPU_INT_IMASK_CNT2STOP_SET)

/*!
 * @brief Counter 3 Overflow interrupt
 */
#define DL_TIMERB_INTERRUPT_CNT3_OVF_EVENT   (BTIMER_CPU_INT_IMASK_CNT3OVF_SET)

/*!
 * @brief Counter 3 Start interrupt
 */
#define DL_TIMERB_INTERRUPT_CNT3_START_EVENT (BTIMER_CPU_INT_IMASK_CNT3STRT_SET)

/*!
 * @brief Counter 3 Stop interrupt
 */
#define DL_TIMERB_INTERRUPT_CNT3_STOP_EVENT (BTIMER_CPU_INT_IMASK_CNT3STOP_SET)

/*!
 * @brief Counter 4 Overflow interrupt
 */
#define DL_TIMERB_INTERRUPT_CNT4_OVF_EVENT   (BTIMER_CPU_INT_IMASK_CNT4OVF_SET)

/*!
 * @brief Counter 4 Start interrupt
 */
#define DL_TIMERB_INTERRUPT_CNT4_START_EVENT (BTIMER_CPU_INT_IMASK_CNT4STRT_SET)

/*!
 * @brief Counter 4 Stop interrupt
 */
#define DL_TIMERB_INTERRUPT_CNT4_STOP_EVENT (BTIMER_CPU_INT_IMASK_CNT4STOP_SET)

/*!
 * @brief Counter 5 Overflow interrupt
 */
#define DL_TIMERB_INTERRUPT_CNT5_OVF_EVENT   (BTIMER_CPU_INT_IMASK_CNT5OVF_SET)

/*!
 * @brief Counter 5 Start interrupt
 */
#define DL_TIMERB_INTERRUPT_CNT5_START_EVENT (BTIMER_CPU_INT_IMASK_CNT5STRT_SET)

/*!
 * @brief Counter 5 Stop interrupt
 */
#define DL_TIMERB_INTERRUPT_CNT5_STOP_EVENT (BTIMER_CPU_INT_IMASK_CNT5STOP_SET)

/*!
 * @brief Counter 6 Overflow interrupt
 */
#define DL_TIMERB_INTERRUPT_CNT6_OVF_EVENT   (BTIMER_CPU_INT_IMASK_CNT6OVF_SET)

/*!
 * @brief Counter 6 Start interrupt
 */
#define DL_TIMERB_INTERRUPT_CNT6_START_EVENT (BTIMER_CPU_INT_IMASK_CNT6STRT_SET)

/*!
 * @brief Counter 6 Stop interrupt
 */
#define DL_TIMERB_INTERRUPT_CNT6_STOP_EVENT (BTIMER_CPU_INT_IMASK_CNT6STOP_SET)

/*!
 * @brief Counter 7 Overflow interrupt
 */
#define DL_TIMERB_INTERRUPT_CNT7_OVF_EVENT   (BTIMER_CPU_INT_IMASK_CNT7OVF_SET)

/*!
 * @brief Counter 7 Start interrupt
 */
#define DL_TIMERB_INTERRUPT_CNT7_START_EVENT (BTIMER_CPU_INT_IMASK_CNT7STRT_SET)

/*!
 * @brief Counter 7 Stop interrupt
 */
#define DL_TIMERB_INTERRUPT_CNT7_STOP_EVENT (BTIMER_CPU_INT_IMASK_CNT7STOP_SET)

/** @}*/

/** @addtogroup DL_TIMERB_EVENT
 *  @{
 */

/*!
 * @brief Counter 0 Overflow event
 */
#define DL_TIMERB_EVENT_CNT0_OVF_EVENT     (BTIMER_GEN_EVENT_IMASK_CNT0OVF_SET)

/*!
 * @brief Counter 0 Start event
 */
#define DL_TIMERB_EVENT_CNT0_START_EVENT  (BTIMER_GEN_EVENT_IMASK_CNT0STRT_SET)

/*!
 * @brief Counter 0 Stop event
 */
#define DL_TIMERB_EVENT_CNT0_STOP_EVENT   (BTIMER_GEN_EVENT_IMASK_CNT0STOP_SET)

/*!
 * @brief Counter 1 Overflow event
 */
#define DL_TIMERB_EVENT_CNT1_OVF_EVENT     (BTIMER_GEN_EVENT_IMASK_CNT1OVF_SET)

/*!
 * @brief Counter 1 Start event
 */
#define DL_TIMERB_EVENT_CNT1_START_EVENT  (BTIMER_GEN_EVENT_IMASK_CNT1STRT_SET)

/*!
 * @brief Counter 1 Stop event
 */
#define DL_TIMERB_EVENT_CNT1_STOP_EVENT   (BTIMER_GEN_EVENT_IMASK_CNT1STOP_SET)

/*!
 * @brief Counter 2 Overflow event
 */
#define DL_TIMERB_EVENT_CNT2_OVF_EVENT     (BTIMER_GEN_EVENT_IMASK_CNT2OVF_SET)

/*!
 * @brief Counter 2 Start event
 */
#define DL_TIMERB_EVENT_CNT2_START_EVENT  (BTIMER_GEN_EVENT_IMASK_CNT2STRT_SET)

/*!
 * @brief Counter 2 Stop event
 */
#define DL_TIMERB_EVENT_CNT2_STOP_EVENT   (BTIMER_GEN_EVENT_IMASK_CNT2STOP_SET)

/*!
 * @brief Counter 3 Overflow event
 */
#define DL_TIMERB_EVENT_CNT3_OVF_EVENT     (BTIMER_GEN_EVENT_IMASK_CNT3OVF_SET)

/*!
 * @brief Counter 3 Start event
 */
#define DL_TIMERB_EVENT_CNT3_START_EVENT  (BTIMER_GEN_EVENT_IMASK_CNT3STRT_SET)

/*!
 * @brief Counter 3 Stop event
 */
#define DL_TIMERB_EVENT_CNT3_STOP_EVENT   (BTIMER_GEN_EVENT_IMASK_CNT3STOP_SET)

/*!
 * @brief Counter 4 Overflow event
 */
#define DL_TIMERB_EVENT_CNT4_OVF_EVENT     (BTIMER_GEN_EVENT_IMASK_CNT4OVF_SET)

/*!
 * @brief Counter 4 Start event
 */
#define DL_TIMERB_EVENT_CNT4_START_EVENT  (BTIMER_GEN_EVENT_IMASK_CNT4STRT_SET)

/*!
 * @brief Counter 4 Stop event
 */
#define DL_TIMERB_EVENT_CNT4_STOP_EVENT   (BTIMER_GEN_EVENT_IMASK_CNT4STOP_SET)

/*!
 * @brief Counter 5 Overflow event
 */
#define DL_TIMERB_EVENT_CNT5_OVF_EVENT     (BTIMER_GEN_EVENT_IMASK_CNT5OVF_SET)

/*!
 * @brief Counter 5 Start event
 */
#define DL_TIMERB_EVENT_CNT5_START_EVENT  (BTIMER_GEN_EVENT_IMASK_CNT5STRT_SET)

/*!
 * @brief Counter 5 Stop event
 */
#define DL_TIMERB_EVENT_CNT5_STOP_EVENT   (BTIMER_GEN_EVENT_IMASK_CNT5STOP_SET)

/*!
 * @brief Counter 6 Overflow event
 */
#define DL_TIMERB_EVENT_CNT6_OVF_EVENT     (BTIMER_GEN_EVENT_IMASK_CNT6OVF_SET)

/*!
 * @brief Counter 6 Start event
 */
#define DL_TIMERB_EVENT_CNT6_START_EVENT  (BTIMER_GEN_EVENT_IMASK_CNT6STRT_SET)

/*!
 * @brief Counter 6 Stop event
 */
#define DL_TIMERB_EVENT_CNT6_STOP_EVENT   (BTIMER_GEN_EVENT_IMASK_CNT6STOP_SET)

/*!
 * @brief Counter 7 Overflow event
 */
#define DL_TIMERB_EVENT_CNT7_OVF_EVENT     (BTIMER_GEN_EVENT_IMASK_CNT7OVF_SET)

/*!
 * @brief Counter 7 Start event
 */
#define DL_TIMERB_EVENT_CNT7_START_EVENT  (BTIMER_GEN_EVENT_IMASK_CNT7STRT_SET)

/*!
 * @brief Counter 7 Stop event
 */
#define DL_TIMERB_EVENT_CNT7_STOP_EVENT   (BTIMER_GEN_EVENT_IMASK_CNT7STOP_SET)

/** @}*/

/* clang-format on */

/*! @enum DL_TIMERB_COUNTER_IDX */
typedef enum {
    /*! BTimer counter index 0 */
    DL_TIMERB_COUNTER_IDX_0 = 0,
    /*! BTimer counter index 1 */
    DL_TIMERB_COUNTER_IDX_1 = 1,
    /*! BTimer counter index 2 */
    DL_TIMERB_COUNTER_IDX_2 = 2,
    /*! BTimer counter index 3 */
    DL_TIMERB_COUNTER_IDX_3 = 3,
    /*! BTimer counter index 4 */
    DL_TIMERB_COUNTER_IDX_4 = 4,
    /*! BTimer counter index 5 */
    DL_TIMERB_COUNTER_IDX_5 = 5,
    /*! BTimer counter index 6 */
    DL_TIMERB_COUNTER_IDX_6 = 6,
    /*! BTimer counter index 7 */
    DL_TIMERB_COUNTER_IDX_7 = 7,
} DL_TIMERB_COUNTER_IDX;

/*! @enum DL_TIMERB_CLOCK_SOURCE */
typedef enum {
    /*! Selects BUSCLK as clock source */
    DL_TIMERB_CLOCK_SOURCE_BUSCLK = (0U << BTIMER_CTL0_CLKSEL_OFS),
    /*! Selects overflow from counter 0 as clock source */
    DL_TIMERB_CLOCK_SOURCE_CNT0_OVERFLOW = (1U << BTIMER_CTL0_CLKSEL_OFS),
    /*! Selects overflow from counter 1 as clock source */
    DL_TIMERB_CLOCK_SOURCE_CNT1_OVERFLOW = (2U << BTIMER_CTL0_CLKSEL_OFS),
    /*! Selects overflow from counter 2 as clock source */
    DL_TIMERB_CLOCK_SOURCE_CNT2_OVERFLOW = (3U << BTIMER_CTL0_CLKSEL_OFS),
    /*! Selects overflow from counter 3 as clock source */
    DL_TIMERB_CLOCK_SOURCE_CNT3_OVERFLOW = (4U << BTIMER_CTL0_CLKSEL_OFS),
    /*! Selects overflow from counter 4 as clock source */
    DL_TIMERB_CLOCK_SOURCE_CNT4_OVERFLOW = (5U << BTIMER_CTL0_CLKSEL_OFS),
    /*! Selects overflow from counter 5 as clock source */
    DL_TIMERB_CLOCK_SOURCE_CNT5_OVERFLOW = (6U << BTIMER_CTL0_CLKSEL_OFS),
    /*! Selects overflow from counter 6 as clock source */
    DL_TIMERB_CLOCK_SOURCE_CNT6_OVERFLOW = (7U << BTIMER_CTL0_CLKSEL_OFS),
    /*! Selects external event 1 as clock source */
    DL_TIMERB_CLOCK_SOURCE_EVENT1 = (8U << BTIMER_CTL0_CLKSEL_OFS),
    /*! Selects external event 2 as clock source */
    DL_TIMERB_CLOCK_SOURCE_EVENT2 = (9U << BTIMER_CTL0_CLKSEL_OFS),
    /*! Selects external event 3 as clock source */
    DL_TIMERB_CLOCK_SOURCE_EVENT3 = (10U << BTIMER_CTL0_CLKSEL_OFS),
    /*! Selects external event 4 as clock source */
    DL_TIMERB_CLOCK_SOURCE_EVENT4 = (11U << BTIMER_CTL0_CLKSEL_OFS),
    /*! Selects external event 5 as clock source */
    DL_TIMERB_CLOCK_SOURCE_EVENT5 = (12U << BTIMER_CTL0_CLKSEL_OFS),
    /*! Selects external event 6 as clock source */
    DL_TIMERB_CLOCK_SOURCE_EVENT6 = (13U << BTIMER_CTL0_CLKSEL_OFS),
    /*! Selects external event 7 as clock source */
    DL_TIMERB_CLOCK_SOURCE_EVENT7 = (14U << BTIMER_CTL0_CLKSEL_OFS),
    /*! Selects subscriber event as clock source */
    DL_TIMERB_CLOCK_SOURCE_SUBSCRIBER_EVENT = (15U << BTIMER_CTL0_CLKSEL_OFS),
} DL_TIMERB_CLOCK_SOURCE;

/*! @enum DL_TIMERB_START_SOURCE */
typedef enum {
    /*! Start source is unused */
    DL_TIMERB_START_SOURCE_UNUSED = (0U << BTIMER_CTL0_STARTSEL_OFS),
    /*! Selects overflow from counter 0 as start source */
    DL_TIMERB_START_SOURCE_CNT0_OVERFLOW = (1U << BTIMER_CTL0_STARTSEL_OFS),
    /*! Selects overflow from counter 1 as start source */
    DL_TIMERB_START_SOURCE_CNT1_OVERFLOW = (2U << BTIMER_CTL0_STARTSEL_OFS),
    /*! Selects overflow from counter 2 as start source */
    DL_TIMERB_START_SOURCE_CNT2_OVERFLOW = (3U << BTIMER_CTL0_STARTSEL_OFS),
    /*! Selects overflow from counter 3 as start source */
    DL_TIMERB_START_SOURCE_CNT3_OVERFLOW = (4U << BTIMER_CTL0_STARTSEL_OFS),
    /*! Selects overflow from counter 4 as start source */
    DL_TIMERB_START_SOURCE_CNT4_OVERFLOW = (5U << BTIMER_CTL0_STARTSEL_OFS),
    /*! Selects overflow from counter 5 as start source */
    DL_TIMERB_START_SOURCE_CNT5_OVERFLOW = (6U << BTIMER_CTL0_STARTSEL_OFS),
    /*! Selects overflow from counter 6 as start source */
    DL_TIMERB_START_SOURCE_CNT6_OVERFLOW = (7U << BTIMER_CTL0_STARTSEL_OFS),
    /*! Selects external event 1 as start source */
    DL_TIMERB_START_SOURCE_EVENT1 = (8U << BTIMER_CTL0_STARTSEL_OFS),
    /*! Selects external event 2 as start source */
    DL_TIMERB_START_SOURCE_EVENT2 = (9U << BTIMER_CTL0_STARTSEL_OFS),
    /*! Selects external event 3 as start source */
    DL_TIMERB_START_SOURCE_EVENT3 = (10U << BTIMER_CTL0_STARTSEL_OFS),
    /*! Selects external event 4 as start source */
    DL_TIMERB_START_SOURCE_EVENT4 = (11U << BTIMER_CTL0_STARTSEL_OFS),
    /*! Selects external event 5 as start source */
    DL_TIMERB_START_SOURCE_EVENT5 = (12U << BTIMER_CTL0_STARTSEL_OFS),
    /*! Selects external event 6 as start source */
    DL_TIMERB_START_SOURCE_EVENT6 = (13U << BTIMER_CTL0_STARTSEL_OFS),
    /*! Selects external event 7 as start source */
    DL_TIMERB_START_SOURCE_EVENT7 = (14U << BTIMER_CTL0_STARTSEL_OFS),
    /*! Selects subscriber event as start source */
    DL_TIMERB_START_SOURCE_SUBSCRIBER_EVENT =
        (15U << BTIMER_CTL0_STARTSEL_OFS),
} DL_TIMERB_START_SOURCE;

/*! @enum DL_TIMERB_STOP_SOURCE */
typedef enum {
    /*! Stop source is unused */
    DL_TIMERB_STOP_SOURCE_UNUSED = (0U << BTIMER_CTL0_STOPSEL_OFS),
    /*! Selects overflow from counter 0 as stop source */
    DL_TIMERB_STOP_SOURCE_CNT0_OVERFLOW = (1U << BTIMER_CTL0_STOPSEL_OFS),
    /*! Selects overflow from counter 1 as stop source */
    DL_TIMERB_STOP_SOURCE_CNT1_OVERFLOW = (2U << BTIMER_CTL0_STOPSEL_OFS),
    /*! Selects overflow from counter 2 as stop source */
    DL_TIMERB_STOP_SOURCE_CNT2_OVERFLOW = (3U << BTIMER_CTL0_STOPSEL_OFS),
    /*! Selects overflow from counter 3 as stop source */
    DL_TIMERB_STOP_SOURCE_CNT3_OVERFLOW = (4U << BTIMER_CTL0_STOPSEL_OFS),
    /*! Selects overflow from counter 4 as stop source */
    DL_TIMERB_STOP_SOURCE_CNT4_OVERFLOW = (5U << BTIMER_CTL0_STOPSEL_OFS),
    /*! Selects overflow from counter 5 as stop source */
    DL_TIMERB_STOP_SOURCE_CNT5_OVERFLOW = (6U << BTIMER_CTL0_STOPSEL_OFS),
    /*! Selects overflow from counter 6 as stop source */
    DL_TIMERB_STOP_SOURCE_CNT6_OVERFLOW = (7U << BTIMER_CTL0_STOPSEL_OFS),
    /*! Selects external event 1 as stop source */
    DL_TIMERB_STOP_SOURCE_EVENT1 = (8U << BTIMER_CTL0_STOPSEL_OFS),
    /*! Selects external event 2 as stop source */
    DL_TIMERB_STOP_SOURCE_EVENT2 = (9U << BTIMER_CTL0_STOPSEL_OFS),
    /*! Selects external event 3 as stop source */
    DL_TIMERB_STOP_SOURCE_EVENT3 = (10U << BTIMER_CTL0_STOPSEL_OFS),
    /*! Selects external event 4 as stop source */
    DL_TIMERB_STOP_SOURCE_EVENT4 = (11U << BTIMER_CTL0_STOPSEL_OFS),
    /*! Selects external event 5 as stop source */
    DL_TIMERB_STOP_SOURCE_EVENT5 = (12U << BTIMER_CTL0_STOPSEL_OFS),
    /*! Selects external event 6 as stop source */
    DL_TIMERB_STOP_SOURCE_EVENT6 = (13U << BTIMER_CTL0_STOPSEL_OFS),
    /*! Selects external event 7 as stop source */
    DL_TIMERB_STOP_SOURCE_EVENT7 = (14U << BTIMER_CTL0_STOPSEL_OFS),
    /*! Selects subscriber event as stop source */
    DL_TIMERB_STOP_SOURCE_SUBSCRIBER_EVENT = (15U << BTIMER_CTL0_STOPSEL_OFS),
} DL_TIMERB_STOP_SOURCE;

/*! @enum DL_TIMERB_RESET_SOURCE */
typedef enum {
    /*! Reset source is unused */
    DL_TIMERB_RESET_SOURCE_UNUSED = (0U << BTIMER_CTL0_RESETSEL_OFS),
    /*! Selects overflow from counter 0 as reset source */
    DL_TIMERB_RESET_SOURCE_CNT0_OVERFLOW = (1U << BTIMER_CTL0_RESETSEL_OFS),
    /*! Selects overflow from counter 1 as reset source */
    DL_TIMERB_RESET_SOURCE_CNT1_OVERFLOW = (2U << BTIMER_CTL0_RESETSEL_OFS),
    /*! Selects overflow from counter 2 as reset source */
    DL_TIMERB_RESET_SOURCE_CNT2_OVERFLOW = (3U << BTIMER_CTL0_RESETSEL_OFS),
    /*! Selects overflow from counter 3 as reset source */
    DL_TIMERB_RESET_SOURCE_CNT3_OVERFLOW = (4U << BTIMER_CTL0_RESETSEL_OFS),
    /*! Selects overflow from counter 4 as reset source */
    DL_TIMERB_RESET_SOURCE_CNT4_OVERFLOW = (5U << BTIMER_CTL0_RESETSEL_OFS),
    /*! Selects overflow from counter 5 as reset source */
    DL_TIMERB_RESET_SOURCE_CNT5_OVERFLOW = (6U << BTIMER_CTL0_RESETSEL_OFS),
    /*! Selects overflow from counter 6 as reset source */
    DL_TIMERB_RESET_SOURCE_CNT6_OVERFLOW = (7U << BTIMER_CTL0_RESETSEL_OFS),
    /*! Selects external event 1 as reset source */
    DL_TIMERB_RESET_SOURCE_EVENT1 = (8U << BTIMER_CTL0_RESETSEL_OFS),
    /*! Selects external event 2 as reset source */
    DL_TIMERB_RESET_SOURCE_EVENT2 = (9U << BTIMER_CTL0_RESETSEL_OFS),
    /*! Selects external event 3 as reset source */
    DL_TIMERB_RESET_SOURCE_EVENT3 = (10U << BTIMER_CTL0_RESETSEL_OFS),
    /*! Selects external event 4 as reset source */
    DL_TIMERB_RESET_SOURCE_EVENT4 = (11U << BTIMER_CTL0_RESETSEL_OFS),
    /*! Selects external event 5 as reset source */
    DL_TIMERB_RESET_SOURCE_EVENT5 = (12U << BTIMER_CTL0_RESETSEL_OFS),
    /*! Selects external event 6 as reset source */
    DL_TIMERB_RESET_SOURCE_EVENT6 = (13U << BTIMER_CTL0_RESETSEL_OFS),
    /*! Selects external event 7 as reset source */
    DL_TIMERB_RESET_SOURCE_EVENT7 = (14U << BTIMER_CTL0_RESETSEL_OFS),
    /*! Selects subscriber event as reset source */
    DL_TIMERB_RESET_SOURCE_SUBSCRIBER_EVENT =
        (15U << BTIMER_CTL0_RESETSEL_OFS),
} DL_TIMERB_RESET_SOURCE;

/*! @enum DL_TIMERB_IIDX */
typedef enum {
    /*! Timer interrupt index for no pending interrupt */
    DL_TIMERB_IIDX_ZERO = BTIMER_CPU_INT_IIDX_STAT_NO_INTR,
    /*! Timer interrupt index for counter 0 period match interrupt */
    DL_TIMERB_IIDX_CNT0_OVERFLOW = BTIMER_CPU_INT_IIDX_STAT_CNT0OVF,
    /*! Timer interrupt index for counter 0 start interrupt */
    DL_TIMERB_IIDX_CNT0_START = BTIMER_CPU_INT_IIDX_STAT_CNT0STRT,
    /*! Timer interrupt index for counter 0 stop interrupt */
    DL_TIMERB_IIDX_CNT0_STOP = BTIMER_CPU_INT_IIDX_STAT_CNT0STOP,
    /*! Timer interrupt index for counter 1 period match interrupt */
    DL_TIMERB_IIDX_CNT1_OVERFLOW = BTIMER_CPU_INT_IIDX_STAT_CNT1OVF,
    /*! Timer interrupt index for counter 1 start interrupt */
    DL_TIMERB_IIDX_CNT1_START = BTIMER_CPU_INT_IIDX_STAT_CNT1STRT,
    /*! Timer interrupt index for counter 1 stop interrupt */
    DL_TIMERB_IIDX_CNT1_STOP = BTIMER_CPU_INT_IIDX_STAT_CNT1STOP,
    /*! Timer interrupt index for counter 2 period match interrupt */
    DL_TIMERB_IIDX_CNT2_OVERFLOW = BTIMER_CPU_INT_IIDX_STAT_CNT2OVF,
    /*! Timer interrupt index for counter 2 start interrupt */
    DL_TIMERB_IIDX_CNT2_START = BTIMER_CPU_INT_IIDX_STAT_CNT2STRT,
    /*! Timer interrupt index for counter 2 stop interrupt */
    DL_TIMERB_IIDX_CNT2_STOP = BTIMER_CPU_INT_IIDX_STAT_CNT2STOP,
    /*! Timer interrupt index for counter 3 period match interrupt */
    DL_TIMERB_IIDX_CNT3_OVERFLOW = BTIMER_CPU_INT_IIDX_STAT_CNT3OVF,
    /*! Timer interrupt index for counter 3 start interrupt */
    DL_TIMERB_IIDX_CNT3_START = BTIMER_CPU_INT_IIDX_STAT_CNT3STRT,
    /*! Timer interrupt index for counter 3 stop interrupt */
    DL_TIMERB_IIDX_CNT3_STOP = BTIMER_CPU_INT_IIDX_STAT_CNT3STOP,
    /*! Timer interrupt index for counter 4 period match interrupt */
    DL_TIMERB_IIDX_CNT4_OVERFLOW = BTIMER_CPU_INT_IIDX_STAT_CNT4OVF,
    /*! Timer interrupt index for counter 4 start interrupt */
    DL_TIMERB_IIDX_CNT4_START = BTIMER_CPU_INT_IIDX_STAT_CNT4STRT,
    /*! Timer interrupt index for counter 4 stop interrupt */
    DL_TIMERB_IIDX_CNT4_STOP = BTIMER_CPU_INT_IIDX_STAT_CNT4STOP,
    /*! Timer interrupt index for counter 5 period match interrupt */
    DL_TIMERB_IIDX_CNT5_OVERFLOW = BTIMER_CPU_INT_IIDX_STAT_CNT5OVF,
    /*! Timer interrupt index for counter 5 start interrupt */
    DL_TIMERB_IIDX_CNT5_START = BTIMER_CPU_INT_IIDX_STAT_CNT5STRT,
    /*! Timer interrupt index for counter 5 stop interrupt */
    DL_TIMERB_IIDX_CNT5_STOP = BTIMER_CPU_INT_IIDX_STAT_CNT5STOP,
    /*! Timer interrupt index for counter 6 period match interrupt */
    DL_TIMERB_IIDX_CNT6_OVERFLOW = BTIMER_CPU_INT_IIDX_STAT_CNT6OVF,
    /*! Timer interrupt index for counter 6 start interrupt */
    DL_TIMERB_IIDX_CNT6_START = BTIMER_CPU_INT_IIDX_STAT_CNT6STRT,
    /*! Timer interrupt index for counter 6 stop interrupt */
    DL_TIMERB_IIDX_CNT6_STOP = BTIMER_CPU_INT_IIDX_STAT_CNT6STOP,
    /*! Timer interrupt index for counter 7 period match interrupt */
    DL_TIMERB_IIDX_CNT7_OVERFLOW = BTIMER_CPU_INT_IIDX_STAT_CNT7OVF,
    /*! Timer interrupt index for counter 7 start interrupt */
    DL_TIMERB_IIDX_CNT7_START = BTIMER_CPU_INT_IIDX_STAT_CNT7STRT,
    /*! Timer interrupt index for counter 7 stop interrupt */
    DL_TIMERB_IIDX_CNT7_STOP = BTIMER_CPU_INT_IIDX_STAT_CNT7STOP,
} DL_TIMERB_IIDX;

/*! @enum  DL_TIMERB_CORE_HALT */
typedef enum {
    /*! Counter will halt immediately, even if the resultant state will result
        in corruption if the system is restarted */
    DL_TIMERB_CORE_HALT_IMMEDIATE =
        (BTIMER_PDBGCTL_FREE_STOP | BTIMER_PDBGCTL_SOFT_IMMEDIATE),
    /*! Counter stops on reaching 0. When the debug halt condition is removed,
        the counter starts from the current value */
    DL_TIMERB_CORE_HALT_DELAYED =
        (BTIMER_PDBGCTL_FREE_STOP | BTIMER_PDBGCTL_SOFT_DELAYED),
    /*! Counter ignores the state of the Core Halted input */
    DL_TIMERB_CORE_HALT_FREE_RUN =
        (BTIMER_PDBGCTL_FREE_RUN | BTIMER_PDBGCTL_SOFT_DELAYED),
} DL_TIMERB_CORE_HALT;

/*!
 *  @brief  Configuration struct for @ref DL_TimerB_initTimer.
 */
typedef struct {
    /*! Counter clock source. One of @ref DL_TIMERB_CLOCK_SOURCE */
    DL_TIMERB_CLOCK_SOURCE clockSource;
    /*! Counter start source. One of @ref DL_TIMERB_START_SOURCE */
    DL_TIMERB_START_SOURCE startSource;
    /*! Counter stop source. One of @ref DL_TIMERB_STOP_SOURCE */
    DL_TIMERB_STOP_SOURCE stopSource;
    /*! Counter reset source. One of @ref DL_TIMERB_RESET_SOURCE */
    DL_TIMERB_RESET_SOURCE resetSource;
    /*! Load value. Range of [0, 65535] */
    uint16_t loadValue;
    /*! Start timer after configuration  */
    bool startTimer;
} DL_TimerB_TimerConfig;

/**
 * @brief Enables the Peripheral Write Enable (PWREN) register for the timer
 *
 *  Before any peripheral registers can be configured by software, the
 *  peripheral itself must be enabled by writing the ENABLE bit together with
 *  the appropriate KEY value to the peripheral's PWREN register.
 *
 * @param timerb              Pointer to the register overlay for the
 *                            peripheral
 */
__STATIC_INLINE void DL_TimerB_enablePower(BTIMER_Regs* const timerb)
{
    timerb->GPRCM.PWREN =
        (BTIMER_PWREN_KEY_UNLOCK_W | BTIMER_PWREN_ENABLE_ENABLE);
}

/**
 * @brief Disables the Peripheral Write Enable (PWREN) register for the timer
 *
 *  When the PWREN.ENABLE bit is cleared, the peripheral's registers are not
 *  accessible for read/write operations.
 *
 *  @note This API does not provide large power savings.
 *
 * @param timerb              Pointer to the register overlay for the
 *                            peripheral
 */
__STATIC_INLINE void DL_TimerB_disablePower(BTIMER_Regs* const timerb)
{
    timerb->GPRCM.PWREN =
        (BTIMER_PWREN_KEY_UNLOCK_W | BTIMER_PWREN_ENABLE_DISABLE);
}

/**
 * @brief Returns if the Peripheral Write Enable (PWREN) register for the timer
 *        is enabled
 *
 *  Before any peripheral registers can be configured by software, the
 *  peripheral itself must be enabled by writing the ENABLE bit together with
 *  the appropriate KEY value to the peripheral's PWREN register.
 *
 *  When the PWREN.ENABLE bit is cleared, the peripheral's registers are not
 *  accessible for read/write operations.
 *
 * @param timerb              Pointer to the register overlay for the
 *                            peripheral
 *
 * @return true if peripheral register access is enabled
 * @return false if peripheral register access is disabled
 */
__STATIC_INLINE bool DL_TimerB_isPowerEnabled(BTIMER_Regs* const timerb)
{
    return ((bool) ((timerb->GPRCM.PWREN & BTIMER_PWREN_ENABLE_MASK) ==
                    BTIMER_PWREN_ENABLE_ENABLE));
}

/**
 * @brief Resets timer peripheral
 *
 * @param timerb              Pointer to the register overlay for the
 *                            peripheral
 */
__STATIC_INLINE void DL_TimerB_reset(BTIMER_Regs* const timerb)
{
    timerb->GPRCM.RSTCTL =
        (BTIMER_RSTCTL_KEY_UNLOCK_W | BTIMER_RSTCTL_RESETASSERT_ASSERT);
}

/**
 * @brief Clear timer peripheral reset assert flag
 *
 * Clear RESETSTKY bit in STAT register that indicates the
 * peripheral was reset.
 *
 * @param timerb              Pointer to the register overlay for the
 *                            peripheral
 */
__STATIC_INLINE void DL_TimerB_clearResetFlag(BTIMER_Regs* const timerb)
{
    timerb->GPRCM.RSTCTL =
        (BTIMER_RSTCTL_KEY_UNLOCK_W | BTIMER_RSTCTL_RESETSTKYCLR_CLR);
}

/**
 * @brief Returns if timer peripheral has been reset
 *
 * @param timerb              Pointer to the register overlay for the
 *                            peripheral
 *
 * @return true if peripheral was reset
 * @return false if peripheral wasn't reset
 */
__STATIC_INLINE bool DL_TimerB_isReset(BTIMER_Regs* const timerb)
{
    return ((bool) ((timerb->GPRCM.STAT & BTIMER_STAT_RESETSTKY_MASK) ==
                    BTIMER_STAT_RESETSTKY_RESET));
}

/**
 *  @brief      Sets timer load value
 *
 *  @note Make sure the timer is not running with @ref DL_TimerB_isRunning()
 *  and @ref DL_TimerB_stopCounter() before calling this API, otherwise this
 *  API will have no effect
 *
 *  @note The counter index should not exceed the number of counters on the
 *  BTimer instance. Passing an out-of-range index will result in reads
 *  returning 0, and writes having no effect
 *
 *  @param[in]  timerb       Pointer to the register overlay for the
 *                           peripheral
 *  @param[in]  idx          Counter index. One of @ref DL_TIMERB_COUNTER_IDX.
 *                           Refer to the device datasheet for the number of
 *                           counters supported on a BTimer instance
 *  @param[in]  value        16-bit load value. Range of [0, 65535]
 *
 *  @sa DL_TimerB_stopCounter
 *  @sa DL_TimerB_isRunning
 */
__STATIC_INLINE void DL_TimerB_setLoadValue(
    BTIMER_Regs* const timerb, DL_TIMERB_COUNTER_IDX idx, uint16_t value)
{
    timerb->CTRREGS[idx].LD = value;
}

/**
 *  @brief      Gets the timer load value
 *
 *  @note The counter index should not exceed the number of counters on the
 *  BTimer instance. Passing an out-of-range index will result in reads
 *  returning 0, and writes having no effect
 *
 *  @param[in]  timerb       Pointer to the register overlay for the
 *                           peripheral
 *  @param[in]  idx          Counter index. One of @ref DL_TIMERB_COUNTER_IDX.
 *                           Refer to the device datasheet for the number of
 *                           counters supported on a BTimer instance
 *
 *  @return     Returns the timer load value. Range of [0, 65535]
 */
__STATIC_INLINE uint16_t DL_TimerB_getLoadValue(
    BTIMER_Regs* const timerb, DL_TIMERB_COUNTER_IDX idx)
{
    return ((uint16_t)(timerb->CTRREGS[idx].LD & BTIMER_LD_VAL_MAXIMUM));
}

/**
 *  @brief      Set timer counter value
 *
 *  @note Make sure the timer is not running with @ref DL_TimerB_isRunning()
 *  and @ref DL_TimerB_stopCounter() before calling this API, otherwise this
 *  API will have no effect
 *
 *  @note The counter index should not exceed the number of counters on the
 *  BTimer instance. Passing an out-of-range index will result in reads
 *  returning 0, and writes having no effect
 *
 *  @param[in]  timerb       Pointer to the register overlay for the
 *                           peripheral
 *  @param[in]  idx          Counter index. One of @ref DL_TIMERB_COUNTER_IDX.
 *                           Refer to the device datasheet for the number of
 *                           counters supported on a BTimer instance
 *  @param[in]  value        16-bit timer counter value. Range of [0, 65535]
 *
 *  @sa DL_TimerB_stopCounter
 *  @sa DL_TimerB_isRunning
 */
__STATIC_INLINE void DL_TimerB_setTimerCount(
    BTIMER_Regs* const timerb, DL_TIMERB_COUNTER_IDX idx, uint16_t value)
{
    timerb->CTRREGS[idx].CNT = value;
}

/**
 *  @brief      Gets the current counter value of the timer
 *
 *  @note It is recommended to check that the counter is not running with
 *  @ref DL_TimerB_isRunning() and @ref DL_TimerB_stopCounter() before calling
 *  this API
 *
 *  @note The counter index should not exceed the number of counters on the
 *  BTimer instance. Passing an out-of-range index will result in reads
 *  returning 0, and writes having no effect
 *
 *  @param[in]  timerb       Pointer to the register overlay for the
 *                           peripheral
 *  @param[in]  idx          Counter index. One of @ref DL_TIMERB_COUNTER_IDX.
 *                           Refer to the device datasheet for the number of
 *                           counters supported on a BTimer instance
 *
 *  @return     Returns the timer counter 16-bit value. Range of [0, 65535]
 *
 *  @sa DL_TimerB_stopCounter
 *  @sa DL_TimerB_isRunning
 */
__STATIC_INLINE uint16_t DL_TimerB_getTimerCount(
    BTIMER_Regs* const timerb, DL_TIMERB_COUNTER_IDX idx)
{
    return ((uint16_t)(timerb->CTRREGS[idx].CNT & BTIMER_CNT_VALUE_MASK));
}

/**
 *  @brief      Initializes all the configurable options for the Basic Timer
 *
 *  @note The counter index should not exceed the number of counters on the
 *  BTimer instance. Passing an out-of-range index will result in reads
 *  returning 0, and writes having no effect
 *
 *  @warning Counter N cannot use the (N+1)th and beyond's counter overflow
 *  as the start, stop, reset, or clock source, to prevent combo loops
 *
 *  @param[in]  timerb       Pointer to the register overlay for the
 *                           peripheral
 *  @param[in]  config       Pointer to the mode configuration struct
 *                           @ref DL_TimerB_TimerConfig.
 *  @param[in]  idx          Counter index. One of @ref DL_TIMERB_COUNTER_IDX.
 *                           Refer to the device datasheet for the number of
 *                           counters supported on a BTimer instance
 */
void DL_TimerB_initTimer(BTIMER_Regs* const timerb,
    DL_TimerB_TimerConfig* const config, DL_TIMERB_COUNTER_IDX idx);

/**
 *  @brief      Starts timer counter
 *
 *  @note The counter index should not exceed the number of counters on the
 *  BTimer instance. Passing an out-of-range index will result in reads
 *  returning 0, and writes having no effect
 *
 *  @param[in]  timerb       Pointer to the register overlay for the
 *                           peripheral
 *  @param[in]  idx          Counter index. One of @ref DL_TIMERB_COUNTER_IDX.
 *                           Refer to the device datasheet for the number of
 *                           counters supported on a BTimer instance
 */
__STATIC_INLINE void DL_TimerB_startCounter(
    BTIMER_Regs* const timerb, DL_TIMERB_COUNTER_IDX idx)
{
    timerb->CTRREGS[idx].CTL0 |= (BTIMER_CTL0_EN_MASK);
}

/**
 *  @brief      Stops timer counter
 *
 *  @note The counter index should not exceed the number of counters on the
 *  BTimer instance. Passing an out-of-range index will result in reads
 *  returning 0, and writes having no effect
 *
 *  @param[in]  timerb       Pointer to the register overlay for the
 *                           peripheral
 *  @param[in]  idx          Counter index. One of @ref DL_TIMERB_COUNTER_IDX.
 *                           Refer to the device datasheet for the number of
 *                           counters supported on a BTimer instance
 */
__STATIC_INLINE void DL_TimerB_stopCounter(
    BTIMER_Regs* const timerb, DL_TIMERB_COUNTER_IDX idx)
{
    timerb->CTRREGS[idx].CTL0 &= ~(BTIMER_CTL0_EN_MASK);
}

/**
 *  @brief      Check if timer is actively running
 *
 *  @note The counter index should not exceed the number of counters on the
 *  BTimer instance. Passing an out-of-range index will result in reads
 *  returning 0, and writes having no effect
 *
 *  @param[in]  timerb      Pointer to the register overlay for the
 *                          peripheral
 *  @param[in]  idx         Counter index. One of @ref DL_TIMERB_COUNTER_IDX.
 *                          Refer to the device datasheet for the number of
 *                          counters supported on a BTimer instance
 *
 *  @return     Returns the running status of the timer
 *
 *  @retval     true  Timer is running
 *  @retval     false Timer is not running
 */
__STATIC_INLINE bool DL_TimerB_isRunning(
    BTIMER_Regs* const timerb, DL_TIMERB_COUNTER_IDX idx)
{
    return ((bool) ((timerb->CTRREGS[idx].CTL0 & BTIMER_CTL0_EN_MASK) ==
                    BTIMER_CTL0_EN_MASK));
}

/**
 *  @brief      Set timer start source
 *
 *  @note Make sure the timer is not running with @ref DL_TimerB_isRunning()
 *  and @ref DL_TimerB_stopCounter() before calling this API, otherwise this
 *  API will have no effect
 *
 *  @note The counter index should not exceed the number of counters on the
 *  BTimer instance. Passing an out-of-range index will result in reads
 *  returning 0, and writes having no effect
 *
 *  @warning Counter N cannot use the (N+1)th and beyond's counter overflow
 *  as the start source, to prevent combo loops
 *
 *  @param[in]  timerb       Pointer to the register overlay for the
 *                           peripheral
 *  @param[in]  idx          Counter index. One of @ref DL_TIMERB_COUNTER_IDX.
 *                           Refer to the device datasheet for the number of
 *                           counters supported on a BTimer instance
 *  @param[in]  startSource  Timer start source.
 *                           One of @ref DL_TIMERB_START_SOURCE
 *
 *  @sa DL_TimerB_stopCounter
 *  @sa DL_TimerB_isRunning
 */
__STATIC_INLINE void DL_TimerB_setTimerStartSource(BTIMER_Regs* const timerb,
    DL_TIMERB_COUNTER_IDX idx, DL_TIMERB_START_SOURCE startSource)
{
    DL_Common_updateReg(&timerb->CTRREGS[idx].CTL0, (uint32_t) startSource,
        BTIMER_CTL0_STARTSEL_MASK);
}

/**
 *  @brief      Gets timer start source
 *
 *  @note The counter index should not exceed the number of counters on the
 *  BTimer instance. Passing an out-of-range index will result in reads
 *  returning 0, and writes having no effect
 *
 *  @param[in]  timerb       Pointer to the register overlay for the
 *                           peripheral
 *  @param[in]  idx          Counter index. One of @ref DL_TIMERB_COUNTER_IDX.
 *                           Refer to the device datasheet for the number of
 *                           counters supported on a BTimer instance
 *
 *  @return     Returns the timer start source.
 *              One of @ref DL_TIMERB_START_SOURCE
 */
__STATIC_INLINE DL_TIMERB_START_SOURCE DL_TimerB_getTimerStartSource(
    BTIMER_Regs* const timerb, DL_TIMERB_COUNTER_IDX idx)
{
    return ((DL_TIMERB_START_SOURCE)(
        timerb->CTRREGS[idx].CTL0 & BTIMER_CTL0_STARTSEL_MASK));
}

/**
 *  @brief      Set timer stop source
 *
 *  @note Make sure the timer is not running with @ref DL_TimerB_isRunning()
 *  and @ref DL_TimerB_stopCounter() before calling this API, otherwise this
 *  API will have no effect
 *
 *  @note The counter index should not exceed the number of counters on the
 *  BTimer instance. Passing an out-of-range index will result in reads
 *  returning 0, and writes having no effect
 *
 *  @warning Counter N cannot use the (N+1)th and beyond's counter overflow
 *  as the stop source, to prevent combo loops
 *
 *  @param[in]  timerb       Pointer to the register overlay for the
 *                           peripheral
 *  @param[in]  idx          Counter index. One of @ref DL_TIMERB_COUNTER_IDX.
 *                           Refer to the device datasheet for the number of
 *                           counters supported on a BTimer instance
 *  @param[in]  stopSource   Timer stop source.
 *                           One of @ref DL_TIMERB_STOP_SOURCE
 *
 *  @sa DL_TimerB_stopCounter
 *  @sa DL_TimerB_isRunning
 */
__STATIC_INLINE void DL_TimerB_setTimerStopSource(BTIMER_Regs* const timerb,
    DL_TIMERB_COUNTER_IDX idx, DL_TIMERB_STOP_SOURCE stopSource)
{
    DL_Common_updateReg(&timerb->CTRREGS[idx].CTL0, (uint32_t) stopSource,
        BTIMER_CTL0_STOPSEL_MASK);
}

/**
 *  @brief      Gets timer stop source
 *
 *  @note The counter index should not exceed the number of counters on the
 *  BTimer instance. Passing an out-of-range index will result in reads
 *  returning 0, and writes having no effect
 *
 *  @param[in]  timerb       Pointer to the register overlay for the
 *                           peripheral
 *  @param[in]  idx          Counter index. One of @ref DL_TIMERB_COUNTER_IDX.
 *                           Refer to the device datasheet for the number of
 *                           counters supported on a BTimer instance
 *
 *  @return     Returns the timer stop source.
 *              One of @ref DL_TIMERB_STOP_SOURCE
 */
__STATIC_INLINE DL_TIMERB_STOP_SOURCE DL_TimerB_getTimerStopSource(
    BTIMER_Regs* const timerb, DL_TIMERB_COUNTER_IDX idx)
{
    return ((DL_TIMERB_STOP_SOURCE)(
        timerb->CTRREGS[idx].CTL0 & BTIMER_CTL0_STOPSEL_MASK));
}

/**
 *  @brief      Set timer reset source
 *
 *  @note Make sure the timer is not running with @ref DL_TimerB_isRunning()
 *  and @ref DL_TimerB_stopCounter() before calling this API, otherwise this
 *  API will have no effect
 *
 *  @note The counter index should not exceed the number of counters on the
 *  BTimer instance. Passing an out-of-range index will result in reads
 *  returning 0, and writes having no effect
 *
 *  @warning Counter N cannot use the (N+1)th and beyond's counter overflow
 *  as the reset source, to prevent combo loops
 *
 *  @param[in]  timerb       Pointer to the register overlay for the
 *                           peripheral
 *  @param[in]  idx          Counter index. One of @ref DL_TIMERB_COUNTER_IDX.
 *                           Refer to the device datasheet for the number of
 *                           counters supported on a BTimer instance
 *  @param[in]  resetSource  Timer reset source.
 *                           One of @ref DL_TIMERB_RESET_SOURCE
 *
 *  @sa DL_TimerB_stopCounter
 *  @sa DL_TimerB_isRunning
 */
__STATIC_INLINE void DL_TimerB_setTimerResetSource(BTIMER_Regs* const timerb,
    DL_TIMERB_COUNTER_IDX idx, DL_TIMERB_RESET_SOURCE resetSource)
{
    DL_Common_updateReg(&timerb->CTRREGS[idx].CTL0, (uint32_t) resetSource,
        BTIMER_CTL0_RESETSEL_MASK);
}

/**
 *  @brief      Gets timer reset source
 *
 *  @note The counter index should not exceed the number of counters on the
 *  BTimer instance. Passing an out-of-range index will result in reads
 *  returning 0, and writes having no effect
 *
 *  @param[in]  timerb       Pointer to the register overlay for the
 *                           peripheral
 *  @param[in]  idx          Counter index. One of @ref DL_TIMERB_COUNTER_IDX.
 *                           Refer to the device datasheet for the number of
 *                           counters supported on a BTimer instance
 *
 *  @return     Returns the timer reset source.
 *              One of @ref DL_TIMERB_RESET_SOURCE
 */
__STATIC_INLINE DL_TIMERB_RESET_SOURCE DL_TimerB_getTimerResetSource(
    BTIMER_Regs* const timerb, DL_TIMERB_COUNTER_IDX idx)
{
    return ((DL_TIMERB_RESET_SOURCE)(
        timerb->CTRREGS[idx].CTL0 & BTIMER_CTL0_RESETSEL_MASK));
}

/**
 *  @brief      Set timer clock source
 *
 *  @note Make sure the timer is not running with @ref DL_TimerB_isRunning()
 *  and @ref DL_TimerB_stopCounter() before calling this API, otherwise this
 *  API will have no effect
 *
 *  @note The counter index should not exceed the number of counters on the
 *  BTimer instance. Passing an out-of-range index will result in reads
 *  returning 0, and writes having no effect
 *
 *  @warning Counter N cannot use the (N+1)th and beyond's counter overflow
 *  as the clock source, to prevent combo loops
 *
 *  @param[in]  timerb       Pointer to the register overlay for the
 *                           peripheral
 *  @param[in]  idx          Counter index. One of @ref DL_TIMERB_COUNTER_IDX.
 *                           Refer to the device datasheet for the number of
 *                           counters supported on a BTimer instance
 *  @param[in]  clockSource  Timer clock source.
 *                           One of @ref DL_TIMERB_CLOCK_SOURCE
 *
 *  @sa DL_TimerB_stopCounter
 *  @sa DL_TimerB_isRunning
 */
__STATIC_INLINE void DL_TimerB_setTimerClockSource(BTIMER_Regs* const timerb,
    DL_TIMERB_COUNTER_IDX idx, DL_TIMERB_CLOCK_SOURCE clockSource)
{
    DL_Common_updateReg(&timerb->CTRREGS[idx].CTL0, (uint32_t) clockSource,
        BTIMER_CTL0_CLKSEL_MASK);
}

/**
 *  @brief      Gets timer clock source
 *
 *  @note The counter index should not exceed the number of counters on the
 *  BTimer instance. Passing an out-of-range index will result in reads
 *  returning 0, and writes having no effect
 *
 *  @param[in]  timerb       Pointer to the register overlay for the
 *                           peripheral
 *  @param[in]  idx          Counter index. One of @ref DL_TIMERB_COUNTER_IDX.
 *                           Refer to the device datasheet for the number of
 *                           counters supported on a BTimer instance
 *
 *  @return     Returns the timer clock source.
 *              One of @ref DL_TIMERB_CLOCK_SOURCE
 */
__STATIC_INLINE DL_TIMERB_CLOCK_SOURCE DL_TimerB_getTimerClockSource(
    BTIMER_Regs* const timerb, DL_TIMERB_COUNTER_IDX idx)
{
    return ((DL_TIMERB_CLOCK_SOURCE)(
        timerb->CTRREGS[idx].CTL0 & BTIMER_CTL0_CLKSEL_MASK));
}

/**
 *  @brief      Enable timer interrupts
 *
 *  @note Configuring interrupts for counters not available on a BTimer
 *  instance will have no effect
 *
 *  @param[in]  timerb         Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  interruptMask  Bit mask of interrupts to enable. Bitwise OR of
 *                             @ref DL_TIMERB_INTERRUPT. Refer to the device
 *                             datasheet for the counters supported on a BTimer
 *                             instance
 */
__STATIC_INLINE void DL_TimerB_enableInterrupt(
    BTIMER_Regs* const timerb, uint32_t interruptMask)
{
    timerb->CPU_INT.IMASK |= interruptMask;
}

/**
 *  @brief      Disable timer interrupts
 *
 *  @param[in]  timerb         Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  interruptMask  Bit mask of interrupts to enable. Bitwise OR of
 *                             @ref DL_TIMERB_INTERRUPT. Refer to the device
 *                             datasheet for the counters supported on a BTimer
 *                             instance
 */
__STATIC_INLINE void DL_TimerB_disableInterrupt(
    BTIMER_Regs* const timerb, uint32_t interruptMask)
{
    timerb->CPU_INT.IMASK &= ~(interruptMask);
}

/**
 *  @brief      Check which timer interrupts are enabled
 *
 *  @param[in]  timerb         Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  interruptMask  Bit mask of interrupts to enable. Bitwise OR of
 *                             @ref DL_TIMERB_INTERRUPT. Refer to the device
 *                             datasheet for the counters supported on a BTimer
 *                             instance
 *
 *  @return     Which of the requested timer interrupts are enabled
 *
 *  @retval     Bitwise OR of @ref DL_TIMERB_INTERRUPT values
 */
__STATIC_INLINE uint32_t DL_TimerB_getEnabledInterrupts(
    BTIMER_Regs* const timerb, uint32_t interruptMask)
{
    return ((uint32_t)(timerb->CPU_INT.IMASK & interruptMask));
}

/**
 *  @brief      Check interrupt flag of enabled timer interrupts
 *
 *  Checks if any of the timer interrupts that were previously enabled are
 *  pending.
 *
 *  @param[in]  timerb        Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  interruptMask  Bit mask of interrupts to enable. Bitwise OR of
 *                             @ref DL_TIMERB_INTERRUPT. Refer to the device
 *                             datasheet for the counters supported on a BTimer
 *                             instance
 *
 *  @return     Which of the requested timer interrupts are pending
 *
 *  @retval     Bitwise OR of @ref DL_TIMERB_INTERRUPT values
 *
 *  @sa         DL_TimerB_enableInterrupt
 */
__STATIC_INLINE uint32_t DL_TimerB_getEnabledInterruptStatus(
    BTIMER_Regs* const timerb, uint32_t interruptMask)
{
    return ((uint32_t)(timerb->CPU_INT.MIS & interruptMask));
}

/**
 *  @brief      Check interrupt flag of any timer interrupt
 *
 *  Checks if any of the timer interrupts are pending. Interrupts do not have
 *  to be previously enabled.
 *
 *  @param[in]  timerb         Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  interruptMask  Bit mask of interrupts to enable. Bitwise OR of
 *                             @ref DL_TIMERB_INTERRUPT. Refer to the device
 *                             datasheet for the counters supported on a BTimer
 *                             instance
 *
 *  @return     Which of the requested timer interrupts are pending
 *
 *  @retval     Bitwise OR of @ref DL_TIMERB_INTERRUPT values
 */
__STATIC_INLINE uint32_t DL_TimerB_getRawInterruptStatus(
    BTIMER_Regs* const timerb, uint32_t interruptMask)
{
    return ((uint32_t)(timerb->CPU_INT.RIS & interruptMask));
}

/**
 *  @brief      Get highest priority pending timer interrupt
 *
 *  Checks if any of the timer interrupts are pending. Interrupts do not have
 *  to be previously enabled.
 *
 *  @param[in]  timerb         Pointer to the register overlay for the
 *                             peripheral
 *
 *  @return     The highest priority pending timer interrupt. One of
 *              @ref DL_TIMERB_IIDX
 */
__STATIC_INLINE DL_TIMERB_IIDX DL_TimerB_getPendingInterrupt(
    BTIMER_Regs* const timerb)
{
    return ((DL_TIMERB_IIDX) timerb->CPU_INT.IIDX);
}

/**
 *  @brief      Clear pending timer interrupts
 *
 *  @param[in]  timerb         Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  interruptMask  Bit mask of interrupts to enable. Bitwise OR of
 *                             @ref DL_TIMERB_INTERRUPT. Refer to the device
 *                             datasheet for the counters supported on a BTimer
 *                             instance
 */
__STATIC_INLINE void DL_TimerB_clearInterruptStatus(
    BTIMER_Regs* const timerb, uint32_t interruptMask)
{
    timerb->CPU_INT.ICLR = interruptMask;
}

/**
 *  @brief Sets the event publisher channel ID
 *
 *  @param[in]  timerb  Pointer to the register overlay for the
 *                      peripheral
 *  @param[in]  chanID  Channel ID number. Valid range 0-15. If ChanID == 0
 *                      publisher is disconnected.
 */
__STATIC_INLINE void DL_TimerB_setPublisherChanID(
    BTIMER_Regs* const timerb, uint8_t chanID)
{
    timerb->FPUB_0 = (chanID & BTIMER_FPUB_0_CHANID_MAXIMUM);
}

/**
 *  @brief Gets the event publisher channel ID
 *
 *  @param[in]  timerb  Pointer to the register overlay for the
 *                      peripheral
 *
 *  @return     Event publisher channel ID
 *
 */
__STATIC_INLINE uint8_t DL_TimerB_getPublisherChanID(BTIMER_Regs* const timerb)
{
    return ((uint8_t)(timerb->FPUB_0 & BTIMER_FPUB_0_CHANID_MASK));
}

/**
 *  @brief Sets the event subscriber channel ID
 *
 *  @param[in]  timerb  Pointer to the register overlay for the
 *                      peripheral
 *  @param[in]  chanID  Channel ID number. Valid range 0-15. If ChanID == 0
 *                      subscriber is disconnected.
 */
__STATIC_INLINE void DL_TimerB_setSubscriberChanID(
    BTIMER_Regs* const timerb, uint8_t chanID)
{
    timerb->FSUB_0 = (chanID & BTIMER_FSUB_0_CHANID_MAXIMUM);
}

/**
 *  @brief Gets the event subscriber channel ID
 *
 *  @param[in]  timerb  Pointer to the register overlay for the
 *                      peripheral
 *
 *  @return     Event subscriber channel ID
 *
 */
__STATIC_INLINE uint8_t DL_TimerB_getSubscriberChanID(
    BTIMER_Regs* const timerb)
{
    return ((uint8_t)(timerb->FSUB_0 & BTIMER_FSUB_0_CHANID_MASK));
}

/**
 *  @brief      Enable timer event
 *
 *  @note Configuring events for counters not available on a BTimer
 *  instance will have no effect
 *
 *  @param[in]  timerb         Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  eventMask      Bit mask of interrupts to enable. Bitwise OR of
 *                             @ref DL_TIMERB_EVENT. Refer to the device
 *                             datasheet for the counters supported on a BTimer
 *                             instance
 */
__STATIC_INLINE void DL_TimerB_enableEvent(
    BTIMER_Regs* const timerb, uint32_t eventMask)
{
    timerb->GEN_EVENT.IMASK |= eventMask;
}

/**
 *  @brief      Disable timer event
 *
 *  @param[in]  timerb         Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  eventMask      Bit mask of interrupts to enable. Bitwise OR of
 *                             @ref DL_TIMERB_EVENT. Refer to the device
 *                             datasheet for the counters supported on a BTimer
 *                             instance
 */
__STATIC_INLINE void DL_TimerB_disableEvent(
    BTIMER_Regs* const timerb, uint32_t eventMask)
{
    timerb->GEN_EVENT.IMASK &= ~(eventMask);
}

/**
 *  @brief      Check which timer events are enabled
 *
 *  @param[in]  timerb         Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  eventMask      Bit mask of interrupts to enable. Bitwise OR of
 *                             @ref DL_TIMERB_EVENT. Refer to the device
 *                             datasheet for the counters supported on a BTimer
 *                             instance
 *
 *  @return     Which of the requested timer interrupts are enabled
 *
 *  @retval     Bitwise OR of @ref DL_TIMERB_EVENT values
 */
__STATIC_INLINE uint32_t DL_TimerB_getEnabledEvents(
    BTIMER_Regs* const timerb, uint32_t eventMask)
{
    return ((uint32_t)(timerb->GEN_EVENT.IMASK & eventMask));
}

/**
 *  @brief      Check event flag of enabled timer event
 *
 *  Checks if any of the timer events that were previously enabled are
 *  pending.
 *
 *  @param[in]  timerb         Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  eventMask      Bit mask of interrupts to enable. Bitwise OR of
 *                             @ref DL_TIMERB_EVENT. Refer to the device
 *                             datasheet for the counters supported on a BTimer
 *                             instance
 *
 *  @return     Which of the requested timer interrupts are pending
 *
 *  @retval     Bitwise OR of @ref DL_TIMERB_EVENT values
 *
 *  @sa         DL_TimerB_enableInterrupt
 */
__STATIC_INLINE uint32_t DL_TimerB_getEnabledEventStatus(
    BTIMER_Regs* const timerb, uint32_t eventMask)
{
    return ((uint32_t)(timerb->GEN_EVENT.MIS & eventMask));
}

/**
 *  @brief      Check interrupt flag of any timer event
 *
 *  Checks if any events are pending. Events do not have to
 *  be previously enabled.
 *
 *  @param[in]  timerb         Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  eventMask      Bit mask of interrupts to enable. Bitwise OR of
 *                             @ref DL_TIMERB_EVENT. Refer to the device
 *                             datasheet for the counters supported on a BTimer
 *                             instance
 *
 *  @return     Which of the requested timer interrupts are pending
 *
 *  @retval     Bitwise OR of @ref DL_TIMERB_EVENT values
 */
__STATIC_INLINE uint32_t DL_TimerB_getRawEventsStatus(
    BTIMER_Regs* const timerb, uint32_t eventMask)
{
    return ((uint32_t)(timerb->GEN_EVENT.RIS & eventMask));
}

/**
 *  @brief      Clear pending timer events
 *
 *  @param[in]  timerb         Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  eventMask      Bit mask of interrupts to enable. Bitwise OR of
 *                             @ref DL_TIMERB_EVENT. Refer to the device
 *                             datasheet for the counters supported on a BTimer
 *                             instance
 */
__STATIC_INLINE void DL_TimerB_clearEventsStatus(
    BTIMER_Regs* const timerb, uint32_t eventMask)
{
    timerb->GEN_EVENT.ICLR |= eventMask;
}

/**
 *  @brief      Configures timer behavior when the core is halted.
 *
 *  @param[in]  timerb   Pointer to the register overlay for the peripheral
 *
 *  @param[in]  haltMode Timer halt behavior. One of @ref DL_TIMERB_CORE_HALT.
 *
 */
__STATIC_INLINE void DL_TimerB_setCoreHaltBehavior(
    BTIMER_Regs* const timerb, DL_TIMERB_CORE_HALT haltMode)
{
    timerb->PDBGCTL = ((uint32_t) haltMode &
                       (BTIMER_PDBGCTL_FREE_MASK | BTIMER_PDBGCTL_SOFT_MASK));
}

/**
 *  @brief      Get timer behavior when the core is halted.
 *
 *  @param[in]  timerb  Pointer to the register overlay for the peripheral
 *
 *  @return     Timer behavior when core is halted. One of
 *              @ref DL_TIMERB_CORE_HALT
 *
 */
__STATIC_INLINE DL_TIMERB_CORE_HALT DL_TimerB_getCoreHaltBehavior(
    BTIMER_Regs* const timerb)
{
    uint32_t haltMode = (timerb->PDBGCTL & (BTIMER_PDBGCTL_FREE_MASK |
                                               BTIMER_PDBGCTL_SOFT_MASK));

    return ((DL_TIMERB_CORE_HALT)(haltMode));
}

#ifdef __cplusplus
}
#endif

#endif /* __MSPM0_HAS_TIMER_B__ */

#endif /* ti_dl_dl_timerb__include */
/** @}*/

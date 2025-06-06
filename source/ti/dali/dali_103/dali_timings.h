/*
 * Copyright (c) 2024, Texas Instruments Incorporated
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
 *  @file       dali_timings.h
 *  @brief      DALI Control Device Timing Requirements
 *  @defgroup   DALI Library (DALI)
 *
 *  @anchor ti_dali_timings_overview
 *  # Overview
 *
 *  The DALI Control Device Timing Requirements
 *
 *  <hr>
 ******************************************************************************/

#ifndef DALI_TIMINGS_H_
#define DALI_TIMINGS_H_

#include <stdint.h>

/* Values are in micro-seconds */

/* Receiver Bit Timings             */
/*! Receiver Half Bit Minimum Time */
#define RECEIVER_HALF_BIT_MIN (333.3f)
/*! Receiver Half Bit Maximum Time */
#define RECEIVER_HALF_BIT_MAX (500.0f)
/*! Receiver Bit Minimum Time */
#define RECEIVER_BIT_MIN (666.7f)
/*! Receiver Bit Maximum Time */
#define RECEIVER_BIT_MAX (1000.0f)

/* Transmitter Bit Timings             */
/*! Transmitter Half Bit Time */
#define TRANSMITTER_HALF_BIT (416.0f)

/* Collision Detection Transmitter Destroy Area */
/*! Transmitter Half Bit Minimum time for Collision Avoidance */
#define TRANSMITTER_CD_HALFBIT_MIN (356.7f)
/*! Transmitter Half Bit Maximum time for Collision Avoidance */
#define TRANSMITTER_CD_HALFBIT_MAX (476.7f)
/*! Transmitter Bit Minimum time for Collision Avoidance */
#define TRANSMITTER_CD_BIT_MIN (723.3f)
/*! Transmitter Bit Maximum time for Collision Avoidance */
#define TRANSMITTER_CD_BIT_MAX (943.3f)
/*! Transmitter Break Time Period */
#define TRANSMITTER_CR_BREAK_TIME (1300)
/*! Transmitter Recovery Timer Period */
#define TRANSMITTER_CR_RECOVERY_TIME (4300)

/* Settling Time */
/*! Settling Time between Forward Frame and Backward Frame */
#define BACKWARD_FRAME_SETTLING_MIN (5500)
/*! Settling time for Forward Frame with priority 1 */
#define FORWARD_FRAME_P1_SETTLING_MIN (13500)
/*! Settling time for Forward Frame with priority 2 */
#define FORWARD_FRAME_P2_SETTLING_MIN (14900)
/*! Settling time for Forward Frame with priority 3 */
#define FORWARD_FRAME_P3_SETTLING_MIN (16300)
/*! Settling time for Forward Frame with priority 4 */
#define FORWARD_FRAME_P4_SETTLING_MIN (17900)
/*! Settling time for Forward Frame with priority 5 */
#define FORWARD_FRAME_P5_SETTLING_MIN (19500)
/*! Send Twice Command Minimum Settling Time */
#define SEND_TWICE_SETTLING_MIN (2400)
/*! Send Twice Command Maximum Settling Time */
#define SEND_TWICE_SETTLING_MAX (94000)

/*! Time in Initialise State*/
#define INITIALISE_STATE_COUNTER (900000000)
/*! Time in Quiescent Mode */
#define QUIESCENT_MODE_COUNTER (900000000)
/*! Time in executing Reset Command */
#define RESET_CMD_COUNTER (300000)

/* Macros not related to Spec */
/* Note: Update the value if Timer Period is changed in sysconfig */
/*! Free Running Timer Period */
#define TIMER_FR_PERIOD (10000)
/*! Time for Settling Timer Tick */
#define SETTLING_TIMER_TICK (2)

/*!
 *@brief Variable to store counter values for Timer
*/
typedef struct {
    /*! Counter for executing Reset command */
    uint16_t resetCounter;
    /*! Counter for Quiescent Mode */
    uint32_t quiescentMode;
    /*! Counter for Initialisation State */
    uint32_t initialisationCounter;
} TickCounter;

/*! Definition of global Tick counter variable */
extern volatile TickCounter gTickCounter;

#endif /* DALI_TIMINGS_H_ */

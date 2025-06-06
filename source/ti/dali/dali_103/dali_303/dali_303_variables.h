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
 *  @file       dali_303_variables.h
 *  @brief      DALI Control Device (Occupancy Sensor) Variable Definition
 *  @defgroup   DALI Library (DALI)
 *
 *  @anchor ti_dali_303_variables_overview
 *  # Overview
 *
 *  The DALI Control Device (Occupancy Sensor) Variable Definition
 *
 *  <hr>
 ******************************************************************************/

#ifndef DALI_303_VARIABLES_H_
#define DALI_303_VARIABLES_H_

#include <stdbool.h>
#include <stdint.h>

/* Flash Vars Definitions               */

/*! DALI Part 303 Instance Type */
#define DALI_303_INSTANCE_TYPE (3)
/*! DALI Part 303 Resolution */
#define DALI_303_RESOLUTION (2)

/*  Default Definitions           */
/*! Default DALI Part 303 Event Filter */
#define DALI_303_EVENT_FILTER_DEFAULT (3)
/*! Default DALI Part 303 Event Priority */
#define DALI_303_EVENT_PRIORITY_DEFAULT (4)
/*! Default DALI Part 303 tDeadtime */
#define DALI_303_DEADTIME_DEFAULT (2)
/*! Default DALI Part 303 tHold */
#define DALI_303_HOLD_DEFAULT (90)
/*! Default DALI Part 303 tReport */
#define DALI_303_REPORT_DEFAULT (20)

/*  Reset Definitions              */
/*! DALI Part 303 Event Filter Reset Value */
#define DALI_303_EVENT_FILTER_RESET (3)
/*! DALI Part 303 Event Priority Reset Value */
#define DALI_303_EVENT_PRIORITY_RESET (4)
/*! DALI Part 303 tDeadtime Reset Value */
#define DALI_303_DEADTIME_RESET (2)
/*! DALI Part 303 tHold Reset Value */
#define DALI_303_HOLD_RESET (90)
/*! DALI Part 303 tReport Reset Value */
#define DALI_303_REPORT_RESET (20)
/*! DALI Part 303 catching Reset Value */
#define DALI_303_CATCHING_RESET false

/* DALI Event Filter */
/*! DALI Part 303 Movement Event Filter*/
#define DALI_303_EVENT_FILTER_MOVEMENT (0x07)

/*  Timer Definitions              */
/*! DALI Part 303 Deadtime Counter increment value in ms */
#define DALI_303_DEADTIME_INCREMENT (50)
/*! DALI Part 303 Hold Counter increment value in ms */
#define DALI_303_HOLD_INCREMENT (10000)
/*! DALI Part 303 Report Counter increment value in ms */
#define DALI_303_REPORT_INCREMENT (1000)
/*! PIR Blindtime increment value in ms*/
#define DALI_303_BLINDTIME_INCREMENT (500)

/*! DALI 303 Timer Period in ms*/
#define TIMER_303_PERIOD (10)
/*!
 *@brief Enum - inputValues for an Occupancy sensor based instance
*/
typedef enum {
    /*! Vacant & No Movement */
    VACANT_NO_MOVEMENT = 0x00,
    /*! Vacant & With Movement */
    VACANT_WITH_MOVEMENT =
        0x55, /* State never reached for a movement Sensor */
    /*! Occupied & No Movement */
    OCCCUPIED_NO_MOVEMENT = 0xAA,
    /*! Occupied & With Movement */
    OCCUPIED_WITH_MOVEMENT = 0xFF
} dali_303_inputValues;

/*!
 *@brief Variable to store counter value for each timer
*/
typedef struct {
    /*! Counter for Deadtime Timer */
    uint16_t deadtimeCounter;
    /*! Counter for Hold Timer */
    uint32_t holdCounter;
    /*! Counter for Report Timer */
    uint32_t reportCounter;
    /*! Counter for Blind Time (PIR Specific)*/
    uint32_t blindTimeCounter;
} dali_303_TimerCounter;

/*!
 * @brief variable to store Occupancy Sensor - instance information
*/
typedef struct {
    /*! Stores the instance Error Byte */
    uint32_t instanceErrorByte;
    /*! Stores the value of deadtime timer*/
    uint8_t tDeadtime;
    /*! Stores the value of hold timer*/
    uint8_t tHold;
    /*! Stores the value of report timer*/
    uint8_t tReport;
    /*! Indicates if movement catching is enabled */
    bool catching;
    /*! Indicates if deadtime Timer is On */
    bool isDeadtimeOn;
} dali_occupancySensorVar;

/*! Definition of global Occupancy Sensor instance variable */
extern volatile dali_occupancySensorVar gOccupancySensorVar1;

/*! Definition of global 303 Timer counter variable */
extern volatile dali_303_TimerCounter g303_TimerCounter;
#endif /* DALI_303_VARIABLES_H_ */

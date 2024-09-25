/*
 * Copyright (c) 2023, Texas Instruments Incorporated
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
 *  @file       SENT.h
 *  @brief      SENT header file
 ******************************************************************************/

#ifndef SENT_SENT_H_
#define SENT_SENT_H_

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "HAL/hal.h"

/* clang-format off */
/* Max tick duration of 90 us */
#define SENT_MAX_TICK_DURATION_US                                 ((uint8_t) 90)
/* Min tick duration of 1 us */
#define SENT_MIN_TICK_DURATION_US                                  ((uint8_t) 1)
/* Table size is 16 for all 16 nibble values (0x0 - 0xF) */
#define SENT_TABLE_SIZE                                                     (16)

/* SYNC pulse is 56 ticks for all frames */
#define SENT_SYNC_TICK_DURATION                                  ((uint16_t) 56)
/* 5 ticks low per pulse */
#define SENT_NIBBLE_LOW_TIME_TICKS                                ((uint16_t) 5)
/* 7 ticks high minimum per pulse */
#define SENT_NIBBLE_HIGH_TIME_MIN_TICKS                           ((uint16_t) 7)
/* 12 ticks per pulse minimum */
#define SENT_NIBBLE_MIN_DURATION_TICKS \
                (SENT_NIBBLE_LOW_TIME_TICKS + SENT_NIBBLE_HIGH_TIME_MIN_TICKS)
/* clang-format on */

typedef enum {
    /*! Define a fixed frame count, pause pulse duration will change */
    SENT_PAUSE_MODE_FIXED_FRAME,
    /*! Define a variable frame count, pause pulse duration will remain constant */
    SENT_PAUSE_MODE_VARIABLE_FRAME,
    /*! Disable pause pulse */
    SENT_PAUSE_MODE_DISABLED,
} SENT_PAUSE_MODE;

typedef enum {
    /*! Send six data nibbles */
    SENT_NUM_DATA_NIBBLES_6 = 6,
} SENT_NUM_DATA_NIBBLES;

typedef enum {
    /*! Initialization state */
    SENT_STATE_INIT,
    /*! Sync pulse state */
    SENT_STATE_SYNC,
    /*! Status pulse state */
    SENT_STATE_STAT,
    /*! Data nibble 0 state */
    SENT_STATE_D0,
    /*! Data nibble 1 state */
    SENT_STATE_D1,
    /*! Data nibble 2 state */
    SENT_STATE_D2,
    /*! Data nibble 3 state */
    SENT_STATE_D3,
    /*! Data nibble 4 state */
    SENT_STATE_D4,
    /*! Data nibble 5 state */
    SENT_STATE_D5,
    /*! CRC4 pulse state */
    SENT_STATE_CRC,
    /*! Pause pulse state */
    SENT_STATE_PAUSE,
} SENT_STATE;

/* Data packet */
typedef struct {
    /*! Status nibble in SENT packet */
    uint8_t status;

    /*! Data nibble 0 in SENT packet */
    uint8_t d0;

    /*! Data nibble 1 in SENT packet */
    uint8_t d1;

    /*! Data nibble 2 in SENT packet */
    uint8_t d2;

    /*! Data nibble 3 in SENT packet */
    uint8_t d3;

    /*! Data nibble 4 in SENT packet */
    uint8_t d4;

    /*! Data nibble 5 in SENT packet */
    uint8_t d5;

    /*! CRC nibble in SENT packet */
    uint8_t crc;

    /*! Pause pulse value in SENT packet */
    uint16_t pause;
} SENT_PACK;

typedef struct {
    /*! Pause mode selection */
    SENT_PAUSE_MODE mode;

    /*! Number of ticks for pause pulse or frame depending on configuration */
    uint16_t numOfTicks;
} SENT_PAUSE;

/* User-configurable parameters */
typedef struct {
    /*! Tick duration in us */
    uint8_t tickDuration;

    /*! Pause configuration struct */
    SENT_PAUSE pauseConfig;

    /*! Number of data nibbles in SENT packet */
    SENT_NUM_DATA_NIBBLES numDataNibbles;
} SENT_config_obj;

/* Run-time parameters */
typedef struct {
    /*! Look-up table for tick count per nibble */
    uint16_t *pPeriodTable;

    /*! Tick duration in us */
    uint8_t tickDuration;

    /*! Pause configuration struct */
    SENT_PAUSE pauseConfig;

    /*! Timer count for SYNC pulse  */
    uint16_t syncCount;

    /*! Timer count for low ticks of each pulse */
    uint16_t lowTimeCount;

    /*! Pointer to current SENT packet */
    SENT_PACK *pCurrentSentPack;

    /*! Pointer to next SENT packet */
    SENT_PACK *pNextSentPack;

    /*! Number of data nibbles in SENT packet */
    uint8_t numOfNibbles;

    /*! Current state of SENT packet */
    SENT_STATE sentState;

    /*! Flag for when to update packet */
    bool updatePack;
} SENT_Handler;

/**
 *  @brief      Initialize frame parameters based on user configuration
 *
 *  @param[in]  pObj        Pointer to user configuration struct
 *  @param[in]  handler     Pointer to parameter struct with calculated
 *                          values based on pObj
 */
bool SENT_init(SENT_config_obj *pObj, SENT_Handler *handler);

/**
 *  @brief      Start SENT transmit
 *
 *  Copy contents of new packet to current packet and start timer
 *
 *  @param[in]  handler     Pointer to parameter struct with calculated
 *                          values based on pObj
 */
void SENT_startTransfer(SENT_Handler *handler);

/**
 *  @brief      Start SENT transmit
 *
 *  Copy contents of new packet to current packet and start timer
 *
 *  @param[in]  handler     Pointer to parameter struct with calculated
 *                          values based on pObj
 */
void SENT_startTransfer(SENT_Handler *handler);

/**
 *  @brief      Update state machine to next state
 *
 *  Update LOAD value for next state and set state to next state
 *
 *  @param[in]  handler     Pointer to parameter struct with calculated
 *                          values based on pObj
 */
void SENT_updateState(SENT_Handler *handler);

/**
 *  @brief      Update pointer to frame data
 *
 *  Copy frame data to current data and calculate CRC and Pause Pulse
 *
 *  @param[in]  handler     Pointer to parameter struct with calculated
 *                          values based on pObj
 *  @param[in]  pNewPack    Pointer to new frame data
 */
void SENT_updateData(SENT_Handler *handler, SENT_PACK *pNewPack);

#endif /* SENT_SENT_H_ */

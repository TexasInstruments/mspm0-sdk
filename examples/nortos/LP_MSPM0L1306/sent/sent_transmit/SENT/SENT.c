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
 *  @file       SENT.c
 *  @brief      SENT source file
 ******************************************************************************/

#include "SENT.h"

uint16_t sentPeriodTable[SENT_TABLE_SIZE];

const static uint8_t CrcLookup[16] = {
    0, 13, 7, 10, 14, 3, 9, 4, 1, 12, 6, 11, 15, 2, 8, 5};

static SENT_PACK gNextSENTPack;
static SENT_PACK gCurrentSENTPack;

static void SENT_calculateCRC(SENT_Handler *handler);
static void SENT_calculatePause(SENT_Handler *handler);

bool SENT_init(SENT_config_obj *pObj, SENT_Handler *handler)
{
    bool status;
    uint8_t index;

    status = true;

    if ((pObj->tickDuration <= SENT_MAX_TICK_DURATION_US) &&
        (pObj->tickDuration >= SENT_MIN_TICK_DURATION_US)) {
        /* Set tick duration if within specified range */
        handler->tickDuration = pObj->tickDuration;
        status &= true;
    } else {
        status &= false;
    }

    /* Assumes timer used to generate SENT signal is configured at 4 MHz */
    /* Set timer counts for sync pulse and low period of pulse  */
    handler->syncCount =
        ((SENT_SYNC_TICK_DURATION * handler->tickDuration) << 2);
    handler->lowTimeCount =
        ((SENT_NIBBLE_LOW_TIME_TICKS * handler->tickDuration) << 2);

    for (index = 0; index < SENT_TABLE_SIZE; index++) {
        /* Fill look-up table with counts*/
        sentPeriodTable[index] =
            (((SENT_NIBBLE_MIN_DURATION_TICKS + index) * handler->tickDuration)
                << 2);
    }

    handler->pPeriodTable = &sentPeriodTable[0];

    handler->numOfNibbles = (uint8_t) pObj->numDataNibbles;

    handler->pCurrentSentPack = &gCurrentSENTPack;

    handler->pNextSentPack = &gNextSENTPack;

    handler->pauseConfig.mode = pObj->pauseConfig.mode;

    handler->pauseConfig.numOfTicks = pObj->pauseConfig.numOfTicks;

    handler->updatePack = false;

    HAL_system_init();
    HAL_Timer_init(handler->syncCount, handler->lowTimeCount);
    HAL_GPIO_init();

    /* Set initial state */
    handler->sentState = SENT_STATE_INIT;

    return status;
}

void SENT_startTransfer(SENT_Handler *handler)
{
    /* Set state to SYNC */
    handler->sentState = SENT_STATE_SYNC;

    /* Copy contents of next packet to current packet */
    memcpy(
        handler->pCurrentSentPack, handler->pNextSentPack, sizeof(SENT_PACK));

    /* Start timer */
    HAL_Timer_start();
}

void SENT_updateData(SENT_Handler *handler, SENT_PACK *newPack)
{
    /* Copy newPack to NextSentPack*/
    memcpy(handler->pNextSentPack, newPack, sizeof(SENT_PACK));
    SENT_calculateCRC(handler);
    SENT_calculatePause(handler);
    handler->updatePack = true;
}

void SENT_updateState(SENT_Handler *handler)
{
    switch (handler->sentState) {
        case SENT_STATE_INIT:
            HAL_Timer_updateLoadVal(handler->syncCount);
            handler->sentState = SENT_STATE_SYNC;
            break;
        case SENT_STATE_SYNC:
            HAL_Timer_updateLoadVal(
                handler->pPeriodTable[handler->pCurrentSentPack->status]);
            handler->sentState = SENT_STATE_STAT;
            break;
        case SENT_STATE_STAT:
            HAL_Timer_updateLoadVal(
                handler->pPeriodTable[handler->pCurrentSentPack->d0]);
            handler->sentState = SENT_STATE_D0;
            break;
        case SENT_STATE_D0:
            HAL_Timer_updateLoadVal(
                handler->pPeriodTable[handler->pCurrentSentPack->d1]);
            handler->sentState = SENT_STATE_D1;
            break;
        case SENT_STATE_D1:
            HAL_Timer_updateLoadVal(
                handler->pPeriodTable[handler->pCurrentSentPack->d2]);
            handler->sentState = SENT_STATE_D2;
            break;
        case SENT_STATE_D2:
            HAL_Timer_updateLoadVal(
                handler->pPeriodTable[handler->pCurrentSentPack->d3]);
            handler->sentState = SENT_STATE_D3;
            break;
        case SENT_STATE_D3:
            HAL_Timer_updateLoadVal(
                handler->pPeriodTable[handler->pCurrentSentPack->d4]);
            handler->sentState = SENT_STATE_D4;
            break;
        case SENT_STATE_D4:
            HAL_Timer_updateLoadVal(
                handler->pPeriodTable[handler->pCurrentSentPack->d5]);
            handler->sentState = SENT_STATE_D5;
            break;
        case SENT_STATE_D5:
            HAL_Timer_updateLoadVal(
                handler->pPeriodTable[handler->pCurrentSentPack->crc]);
            handler->sentState = SENT_STATE_CRC;
            break;
        case SENT_STATE_CRC:

            if (handler->pauseConfig.mode == SENT_PAUSE_MODE_DISABLED ||
                (handler->pCurrentSentPack->pause == 0)) {
                HAL_Timer_updateLoadVal(handler->syncCount);
                handler->sentState = SENT_STATE_SYNC;
            } else {
                HAL_Timer_updateLoadVal(handler->pCurrentSentPack->pause);
                handler->sentState = SENT_STATE_PAUSE;
            }
            break;
        case SENT_STATE_PAUSE:
            HAL_Timer_updateLoadVal(handler->syncCount);
            handler->sentState = SENT_STATE_SYNC;
            break;
        default:
            break;
    }

    if (handler->sentState == SENT_STATE_SYNC) {
        if (handler->updatePack == true) {
            memcpy(handler->pCurrentSentPack, handler->pNextSentPack,
                sizeof(SENT_PACK));
            handler->updatePack = false;
        }
    }
}

/* static functions */
void SENT_calculateCRC(SENT_Handler *handler)
{
    uint8_t crc;
    uint8_t numOfNibbles;
    uint8_t *pData;

    crc = 0x5; /* Initialize checksum with seed "0101" */

    numOfNibbles = handler->numOfNibbles;
    pData        = &(handler->pNextSentPack->d0);

    for (uint8_t i = 0; i < numOfNibbles; i++) {
        crc = *(pData++) ^ CrcLookup[crc];
    }

    crc = 0x0 ^ CrcLookup[crc];

    handler->pNextSentPack->crc = crc;
}

void SENT_calculatePause(SENT_Handler *handler)
{
    uint16_t pauseTicks;
    uint16_t timerCnt;

    switch (handler->pauseConfig.mode) {
        case SENT_PAUSE_MODE_VARIABLE_FRAME:
            pauseTicks = (handler->pauseConfig.numOfTicks);
            handler->pNextSentPack->pause =
                ((pauseTicks * handler->tickDuration) << 2);
            break;
        case SENT_PAUSE_MODE_FIXED_FRAME:
            pauseTicks = (handler->pauseConfig.numOfTicks);

            timerCnt = (handler->syncCount);
            timerCnt +=
                (handler->pPeriodTable[handler->pNextSentPack->status]);
            timerCnt += (handler->pPeriodTable[handler->pNextSentPack->d0]);
            timerCnt += (handler->pPeriodTable[handler->pNextSentPack->d1]);
            timerCnt += (handler->pPeriodTable[handler->pNextSentPack->d2]);
            timerCnt += (handler->pPeriodTable[handler->pNextSentPack->d3]);
            timerCnt += (handler->pPeriodTable[handler->pNextSentPack->d4]);
            timerCnt += (handler->pPeriodTable[handler->pNextSentPack->d5]);
            timerCnt += (handler->pPeriodTable[handler->pNextSentPack->crc]);

            /*
             * Only insert pause ticks if the SENT package still has room for
             * pause ticks to be padded with. Otherwise, set number of pause
             * ticks to 0.
             */
            if (timerCnt <= (((uint32_t) handler->pauseConfig.numOfTicks -
                                 (uint32_t) handler->lowTimeCount)
                                << 2)) {
                handler->pNextSentPack->pause =
                    (((uint32_t)((uint32_t) pauseTicks *
                                 (uint32_t) handler->tickDuration)
                         << 2) -
                        timerCnt);
            } else {
                handler->pNextSentPack->pause = 0;
            }

            break;
        case SENT_PAUSE_MODE_DISABLED:
            handler->pNextSentPack->pause = 0;
            break;
        default:
            break;
    }
}

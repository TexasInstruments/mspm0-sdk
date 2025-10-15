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
 
#include "sourceCurrentLimit.h"

/* Include the IQmath Library */
#include <ti/iqmath/include/IQmathLib.h>

#include "main.h"


void sourceCurrentLimitInit(SOURCE_CURRENT_LIMIT_T *pSourceCurrentLimit)
{
    pSourceCurrentLimit->state = SOURCE_CURRENT_BELOW_LIMIT;
    pSourceCurrentLimit->prevState = pSourceCurrentLimit->state;

    pSourceCurrentLimit->transition = SOURCE_CURRENT_NO_TRANSITION;
}


void sourceCurrentLimitRun(SOURCE_CURRENT_LIMIT_T *pSourceCurrentLimit)
{
    int32_t sourceCurrent,sourceCurrentLmtThreshhold;

    sourceCurrent = *(pSourceCurrentLimit->pSourceCurrent);
    sourceCurrentLmtThreshhold = _IQmpy(pSourceCurrentLimit->sourceCurrentLimit
                                        ,SOURCE_CURRENT_LIMIT_THRESHHOLD);

    switch(pSourceCurrentLimit->state)
    {
    case SOURCE_CURRENT_BELOW_LIMIT:

        if(sourceCurrent >= pSourceCurrentLimit->sourceCurrentLimit)
        {
            pSourceCurrentLimit->state = SOURCE_CURRENT_ABOVE_LIMIT;
            pSourceCurrentLimit->transition =
                    SOURCE_CURRENT_TRANSITION_TO_CURRENT_LIMIT;
        }
        break;

    case SOURCE_CURRENT_ABOVE_LIMIT:

        if((sourceCurrent < sourceCurrentLmtThreshhold) ||
                !pSourceCurrentLimit->flags.b.enable)
        {
            pSourceCurrentLimit->state = SOURCE_CURRENT_BELOW_LIMIT;
            pSourceCurrentLimit->transition =
                    SOURCE_CURRENT_TRANSITION_TO_NORMAL_CURRENT;
        }
        break;

    default:

        pSourceCurrentLimit->state = SOURCE_CURRENT_BELOW_LIMIT;
        break;
    }


    if(pSourceCurrentLimit->prevState == pSourceCurrentLimit->state)
    {
        pSourceCurrentLimit->transition = SOURCE_CURRENT_NO_TRANSITION;
    }

    pSourceCurrentLimit->prevState = pSourceCurrentLimit->state;
}

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


/* Include the IQmath Library */
#include <ti/iqmath/include/IQmathLib.h>

#include "stallDetect.h"
#include "appDefs.h"

void stallDetectInit(STALL_DETECT_T *pStall)
{
    lowBemfStallDetectInit(&pStall->lowBemfStall);

    noMotorStallDetectInit(&pStall->noMotorStall);

    abnormalSpeedStallDetectInit(&pStall->abnormalSpeedStall);
}

void stallDetectRun(STALL_DETECT_T *pStall)
{

    /* Low BEMF Stall Detection */
    if(pStall->lowBemfStall.flags.b.enable)
    {
        lowBemfStallDetectRun(&pStall->lowBemfStall);

        if(getLowBemfStallStatus(&pStall->lowBemfStall))
        {
            pStall->status = LOW_BEMF_STALL;
        }
    }

    /* Under Current Stall Detection */
    if(pStall->noMotorStall.flags.b.enable)
    {
        noMotorStallDetectRun(&pStall->noMotorStall);

        if(getNoMotorStallDetectStatus(&pStall->noMotorStall))
        {
            pStall->status = NO_MOTOR_STALL;
        }
    }

}

void stallDetectLowPriorityRun(STALL_DETECT_T *pStall)
{

    if(pStall->abnormalSpeedStall.flags.b.enable)
    {
        abnormalSpeedStallDetectRun(&pStall->abnormalSpeedStall);

        if(getAbnormalSpeedStallDetectStatus(&pStall->abnormalSpeedStall))
        {
            pStall->status = ABNORMAL_SPEED_STALL;
        }
    }

}

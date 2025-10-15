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

#include "sourceVoltageLimit.h"
#include "focHALInterface.h"
#include "appDefs.h"

void sourceVoltageLimitInit(SOURCE_VOLTAGE_LIMIT_T *pSourceVoltageLimit)
{
    pSourceVoltageLimit->count = 0;
}

void sourceVoltageLimitRun(SOURCE_VOLTAGE_LIMIT_T *pSourceVoltageLimit)
{
    int32_t
    Vdc;

    _Bool underVoltageFaultClear = 0, overVoltageFaultClear = 0;

    Vdc = *(pSourceVoltageLimit->pVdc);

    underVoltageFaultClear =
            (pSourceVoltageLimit->flags.b.underVoltageFaultStatus &&
                    pSourceVoltageLimit->flags.b.underVoltageFaultAutoClear &&
                    (Vdc >= pSourceVoltageLimit->underVoltageFaultClearThreshold ||
                            !pSourceVoltageLimit->flags.b.underVoltageFaultEnable));

    overVoltageFaultClear =
            (pSourceVoltageLimit->flags.b.overVoltageFaultStatus &&
                    pSourceVoltageLimit->flags.b.overVoltageFaultAutoClear &&
                    (Vdc <= pSourceVoltageLimit->overVoltageFaultClearThreshold ||
                            !pSourceVoltageLimit->flags.b.overVoltageFaultEnable));

    if((Vdc < pSourceVoltageLimit->minDCBusVoltage) &&
            pSourceVoltageLimit->flags.b.underVoltageFaultEnable)
    {
        if(pSourceVoltageLimit->count >= pSourceVoltageLimit->countMax)
        {
            pSourceVoltageLimit->faultStatus = UNDER_VOLTAGE;
            pSourceVoltageLimit->flags.b.underVoltageFaultStatus = TRUE;
        }
        else
        {
            pSourceVoltageLimit->count++;
        }
    }
    else if((Vdc > pSourceVoltageLimit->maxDCBusVoltage) &&
            pSourceVoltageLimit->flags.b.overVoltageFaultEnable)
    {
        if(pSourceVoltageLimit->count >= pSourceVoltageLimit->countMax)
        {
            pSourceVoltageLimit->faultStatus = OVER_VOLTAGE;
            pSourceVoltageLimit->flags.b.overVoltageFaultStatus = TRUE;
        }
        else
        {
            pSourceVoltageLimit->count++;
        }
    }

    if(underVoltageFaultClear)
    {
        pSourceVoltageLimit->flags.b.underVoltageFaultStatus = FALSE;
    }

    if(overVoltageFaultClear)
    {
        pSourceVoltageLimit->flags.b.overVoltageFaultStatus = FALSE;
    }

    if(!(pSourceVoltageLimit->flags.b.underVoltageFaultStatus ||
            pSourceVoltageLimit->flags.b.overVoltageFaultStatus))
    {
        if((Vdc < pSourceVoltageLimit->overVoltageFaultClearThreshold) &&
                (Vdc > pSourceVoltageLimit->underVoltageFaultClearThreshold))
        {
            pSourceVoltageLimit->count = 0;
            pSourceVoltageLimit->faultStatus = NO_VOLTAGE_FAULT;
        }
    }
}




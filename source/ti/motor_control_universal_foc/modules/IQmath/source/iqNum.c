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

#include "iqNum.h"

/* Include the IQmath Library */
#include <ti/iqmath/include/IQmathLib.h>

void float2IQ(IQ_NUM_T *pIqNum, const float setValue)
{
    float scaledNum;

    int16_t scaling;

    scaledNum = setValue;

    for(scaling = 0; scaling < GLOBAL_IQ; scaling++)
    {
        if(scaledNum >= ( (float)(_IQ(1.0)) ) )
        {
            break;
        }
        else
        {
            scaledNum += scaledNum;
        }
    }

    pIqNum->scaledNum = (int32_t)(scaledNum);
    pIqNum->iqScaling = scaling;
}

void float2IQx(int32_t *pIqxNum, const float setValue, const int16_t iqScaling)
{
    float scaledNum;
    int16_t scaling;

    scaledNum = setValue;

    for(scaling = 0; scaling < iqScaling; scaling++)
    {
        scaledNum += scaledNum;
    }

    *pIqxNum = scaledNum;
}

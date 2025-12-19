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

#include <ti/iqmath/include/IQmathLib.h>
#include "ti_msp_dl_config.h"

#define PI (3.1415926536)

volatile float gErrorTolerance;
volatile bool gTestFailure;

int main(void)
{
    /* floating point variable to verify results */
    volatile float res;
    /* IQ variables using global type */
    _iq qA, qB, qC;
    /* IQ variables using IQ8 type */
    _iq8 q8A, q8B, q8C;
    /* IQ variables using IQ15 type */
    _iq15 q15A, q15C;

    gErrorTolerance = 0.01;
    gTestFailure = false;

    SYSCFG_DL_init();

    /* Basic global IQ operations. */
    qA  = _IQ(1.0);
    qB  = _IQ(2.5);
    qC  = qA + qB;
    /* 3.5 = 1.0 + 2.5 */
    res = _IQtoF(qC);
    if (!((res>=(3.5-(res*gErrorTolerance))) && (res <= (3.5+(res*gErrorTolerance))))){
        gTestFailure = true;
    };


    qC  = qC - _IQmpy2(qA);
    /* 1.5 = 3.5 - 2*(1.0) */
    res = _IQtoF(qC);
    if (!((res>=(1.5-(res*gErrorTolerance))) && (res <= (1.5+(res*gErrorTolerance))))){
         gTestFailure = true;
    };

    qC  = _IQmpy(qB, qC);
    /* 3.75 = 2.5 * 1.5 */
    res = _IQtoF(qC);
    if (!((res>=(3.75-(res*gErrorTolerance))) && (res <= (3.75+(res*gErrorTolerance))))){
         gTestFailure = true;
    };

    qC  = _IQdiv(qC, qB);
    /* 1.5 = 3.75 / 2.5 */
    res = _IQtoF(qC);
    if (!((res>=(1.5-(res*gErrorTolerance))) && (res <= (1.5+(res*gErrorTolerance))))){
         gTestFailure = true;
    };

    qC  = _IQsqrt(qB);
    /* 1.58113885 = sqrt(2.5) */
    res = _IQtoF(qC);
    if (!((res>=(1.58113885-(res*gErrorTolerance))) && (res <= (1.58113885+(res*gErrorTolerance))))){
         gTestFailure = true;
    };
                       
    /* Trigonometric global IQ operations. */
    qA  = _IQ(PI / 4.0);
    qB  = _IQ(0.5);
    qC  = _IQsin(qA);
    /* 0.707106709 = sin(PI/4) */
    res = _IQtoF(qC);
    if (!((res>=(0.707106709-(res*gErrorTolerance))) && (res <= (0.707106709+(res*gErrorTolerance))))){
         gTestFailure = true;
    };

    qC  = _IQcos(qA);
    /* 0.707106769 = cos(PI/4) */
    res = _IQtoF(qC);
    if (!((res>=(0.707106709-(res*gErrorTolerance))) && (res <= (0.707106709+(res*gErrorTolerance))))){
         gTestFailure = true;
    };

    qC  = _IQatan(qB);
    /* 0.463647604 = atan(0.5) */
    res = _IQtoF(qC);
    if (!((res>=(0.463647604-(res*gErrorTolerance))) && (res <= (0.463647604+(res*gErrorTolerance))))){
         gTestFailure = true;
    };

    /* Exponential global IQ operations. */
    qA  = _IQ(2.71828);
    qB  = _IQ(0.5);
    qC  = _IQlog(qA);
    /* 0.9999999225 = ln(e) */
    res = _IQtoF(qC);
    if (!((res>=(0.9999999225-(res*gErrorTolerance))) && (res <= (0.9999999225+(res*gErrorTolerance))))){
         gTestFailure = true;
    };

    qC  = _IQexp(qB);
    /* 1.64872134 = e^0.5 */
    res = _IQtoF(qC);
    if (!((res>=(1.64872134-(res*gErrorTolerance))) && (res <= (1.64872134+(res*gErrorTolerance))))){
         gTestFailure = true;
    };

    /* Basic explicit type IQ8 operations. */
    q8A = _IQ8(1.0);
    q8B = _IQ8(2.5);
    q8C = q8A + q8B;
    /* 3.5 = 1.0 + 2.5 */
    res = _IQ8toF(q8C);
    if (!((res>=(3.5-(res*gErrorTolerance))) && (res <= (3.5+(res*gErrorTolerance))))){
         gTestFailure = true;
    };

    q8C = q8C - _IQmpy2(q8A);
    /* 1.5 = 3.5 - 2*(1.0) */
    res = _IQ8toF(q8C);
    if (!((res>=(1.5-(res*gErrorTolerance))) && (res <= (1.5+(res*gErrorTolerance))))){
         gTestFailure = true;
    };

    q8C = _IQ8mpy(q8B, q8C);
    /* 3.75 = 2.5 * 1.5 */
    res = _IQ8toF(q8C);
    if (!((res>=(3.75-(res*gErrorTolerance))) && (res <= (3.75+(res*gErrorTolerance))))){
         gTestFailure = true;
    };

    q8C = _IQ8div(q8C, q8B);
    /* 1.5 = 3.75 / 2.5 */
    res = _IQ8toF(q8C);
    if (!((res>=(1.5-(res*gErrorTolerance))) && (res <= (1.5+(res*gErrorTolerance))))){
         gTestFailure = true;
    };

    q8C = _IQ8sqrt(q8B);
    /* 1.58203125 = sqrt(2.5) */
    res = _IQ8toF(q8C);
    if (!((res>=(1.58203125-(res*gErrorTolerance))) && (res <= (1.58203125+(res*gErrorTolerance))))){
         gTestFailure = true;
    };

    /* Trigonometric explicit type IQ15 operations. */
    q15A = _IQ15(PI / 4.0);
    q15C = _IQ15sin(q15A);
    /* 0.707061768 = sin(PI/4) */
    res  = _IQ15toF(q15C);
    if (!((res>=(0.707061768-(res*gErrorTolerance))) && (res <= (0.707061768+(res*gErrorTolerance))))){
         gTestFailure = true;
    };


    q15C = _IQ15cos(q15A);
    /* 0.707122803 = cos(PI/4) */
    res  = _IQ15toF(q15C);
    if (!((res>=(0.707061768-(res*gErrorTolerance))) && (res <= (0.707061768+(res*gErrorTolerance))))){
         gTestFailure = true;
    };

    /* Explicit type IQ8 to Global IQ conversion with saturation check. */
    q8A = _IQ8(1.0);
    q8B = _IQ8(16.0);
    qC  = _IQ8toIQ(_IQsat(q8A, _IQtoQ8(MAX_IQ_POS), _IQtoQ8(MAX_IQ_NEG)));
    /* _IQ8(1.0) -> _IQ(1.0) (q8A does not saturate) */
    res = _IQtoF(qC);
    if (!((res>=(1.0-(res*gErrorTolerance))) && (res <= (1.0+(res*gErrorTolerance))))){
         gTestFailure = true;
    };

    qC  = _IQ8toIQ(_IQsat(q8B, _IQtoQ8(MAX_IQ_POS), _IQtoQ8(MAX_IQ_NEG)));
    /* _IQ8(16.0) -> ~MAX_IQ_POS (q8A saturates to maximum positive _IQ value) */
    res = _IQtoF(qC);
    if (!((res>=(16.0-(res*gErrorTolerance))) && (res <= (16.0+(res*gErrorTolerance))))){
         gTestFailure = true;
    };

    /*
     * Stop at the break point to verify values passed test.
     * gTestFailure = true if any results is not within error margin
     * gErrorTolerance = fraction of acceptable error, default set to 1%
     */
    __BKPT(0);

    while (1) {
        __WFI();
    }
}


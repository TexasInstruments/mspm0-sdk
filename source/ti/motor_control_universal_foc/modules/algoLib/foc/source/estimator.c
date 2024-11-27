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

#include "estimator.h"


void estimatorRun(ESTIMATOR_T *pEstim)
{
    int32_t
        Laa,
        Lab,
        Lba,
        Lbb,
        L1Cos2q,
        L1Sin2q;

    MC_ALPHABETA_T
        iAlphaBeta,
        vAlphaBeta,
        vResAlphaBeta,
        vIndAlphaBeta,
        fluxAlphaBeta;

    MC_SINCOS_T
        sinCos;

    MOTOR_T
        motor;

    vAlphaBeta = *(pEstim->pVAlphaBeta);
    iAlphaBeta = *(pEstim->pIAlphaBeta);
    motor = *(pEstim->pMotor);

    /* Calculate Resistive Drop:
     * Since resistive drop is scaled to voltage, considering scaling at this
     * stage gives voltage in GLOBAL_IQ format without overflows */

    vResAlphaBeta.alpha =
            (_IQmpy_mathacl(iAlphaBeta.alpha, motor.rs)) <<
                (GLOBAL_IQ - RS_IQ_SCALING);

    vResAlphaBeta.beta =
            (_IQmpy_mathacl(iAlphaBeta.beta, motor.rs)) <<
                (GLOBAL_IQ - RS_IQ_SCALING);

    /* Calculate Flux:
     * Since the actual calculation is not on flux but flux/deltaT, it is
     * unlikely that it will fit in the proper number. Hence, this flux is
     * scaled with LsDt Scaling */
    if(motor.saliency)
    {
        sinCos.sin =
            _IQmpy_mathacl(pEstim->pAngleTrackingPLLEstim->sinCosFluxAngle.sin,
                    pEstim->pAngleTrackingPLLEstim->sinCosFluxAngle.cos) << 1;

        sinCos.cos =
            (_IQmpy_mathacl(pEstim->pAngleTrackingPLLEstim->sinCosFluxAngle.cos,
                    pEstim->pAngleTrackingPLLEstim->sinCosFluxAngle.cos) << 1) -
                    _IQ(1.0);

        L1Cos2q = _IQmpy_mathacl(motor.L1Dt, sinCos.cos);
        L1Sin2q = _IQmpy_mathacl(motor.L1Dt, sinCos.sin);

        Laa = motor.L0Dt + L1Cos2q;
        Lab = L1Sin2q;
        Lba = L1Sin2q;
        Lbb = motor.L0Dt - L1Cos2q;

        fluxAlphaBeta.alpha =
                _IQmpy_mathacl(iAlphaBeta.alpha, Laa) + _IQmpy_mathacl(iAlphaBeta.beta, Lab);

        fluxAlphaBeta.beta =
                _IQmpy_mathacl(iAlphaBeta.alpha, Lba) + _IQmpy_mathacl(iAlphaBeta.beta, Lbb);
    }
    else
    {
        fluxAlphaBeta.alpha =
                _IQmpy_mathacl(iAlphaBeta.alpha, motor.L0Dt);

        fluxAlphaBeta.beta =
                _IQmpy_mathacl(iAlphaBeta.beta, motor.L0Dt);
    }

    /* Calculate Inductive Drop:
     * Flux is scaled to LsDt scaling. Since expectation is that the voltage is
     * in GLOBAL_IQ format, it needs to be converted to this format */

    vIndAlphaBeta.alpha =
            (fluxAlphaBeta.alpha - pEstim->prevFluxAlphaBeta.alpha) <<
            (GLOBAL_IQ - LS_IQ_SCALING);

    vIndAlphaBeta.beta =
            (fluxAlphaBeta.beta - pEstim->prevFluxAlphaBeta.beta) <<
            (GLOBAL_IQ - LS_IQ_SCALING);

    /* Calculate BEMF */
    pEstim->eAlphaBeta.alpha = vAlphaBeta.alpha -
                                vResAlphaBeta.alpha -
                                vIndAlphaBeta.alpha;

    pEstim->eAlphaBeta.beta = vAlphaBeta.beta -
                                vResAlphaBeta.beta -
                                vIndAlphaBeta.beta;

    /* Calculate speed and angle based on semi close loop estimator */
    angleTrackingPLLRun(pEstim->pAngleTrackingPLLEstim);
    /* Populate the previous flux value */
    pEstim->prevFluxAlphaBeta = fluxAlphaBeta;

}

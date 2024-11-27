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

#include "mcLib.h"
#include "focHALInterface.h"

/*! @brief Deglitch count */
#define SECTOR_DEGLITCH_COUNT   8

/******************************************************************************/

void MC_ClarkeTransformNoZeroSeq(MC_ALPHABETA_T *pAlphaBeta,
                                 const MC_ABC_T *pABC)
{
    pAlphaBeta->alpha = pABC->a;

    pAlphaBeta->beta = _IQmpy_mathacl((pABC->b - pABC->c), ONE_BY_SQRT_THREE);
}
/******************************************************************************/

void MC_ClarkeTransform(MC_ALPHABETA_T *pAlphaBeta,
                        const MC_ABC_T *pABC)
{
    pAlphaBeta->alpha = _IQmpy_mathacl(( (pABC->a << 1) -
            (pABC->b + pABC->c) ),
                               ONE_BY_THREE);

    pAlphaBeta->beta = _IQmpy_mathacl((pABC->b - pABC->c), ONE_BY_SQRT_THREE);
}
/******************************************************************************/

void MC_SVM_ClarkeTransform(MC_ALPHABETA_T *pAlphaBeta,
                            const MC_ABC_T *pABC)
{
    pAlphaBeta->alpha = _IQmpy_mathacl(( (pABC->a << 1) -
            (pABC->b + pABC->c) ),
                               ONE_BY_SQRT_THREE);

    pAlphaBeta->beta = (pABC->b - pABC->c);
}

/******************************************************************************/

void MC_InverseClarkeTransform(MC_ABC_T *pABC,
                               const MC_ALPHABETA_T *pAlphaBeta)
{
    int32_t
    negAlphaByTwo,
    betaSqrtThreeByTwo;

    negAlphaByTwo = -(pAlphaBeta->alpha >> 1);
    betaSqrtThreeByTwo = _IQmpy_mathacl(pAlphaBeta->beta, SQRT_THREE_BY_TWO);

    pABC->a = pAlphaBeta->alpha;
    pABC->b = negAlphaByTwo + betaSqrtThreeByTwo;
    pABC->c = negAlphaByTwo - betaSqrtThreeByTwo;
}
/******************************************************************************/

void MC_ParkTransform(MC_DQ_T *pDQ,
                      const MC_ALPHABETA_T *pAlphaBeta,
                      const MC_SINCOS_T *pSinCos)
{
    pDQ->d = _IQmpy_mathacl(pAlphaBeta->alpha, pSinCos->cos) +
            _IQmpy_mathacl(pAlphaBeta->beta, pSinCos->sin);

    pDQ->q = _IQmpy_mathacl(pAlphaBeta->beta, pSinCos->cos) -
            _IQmpy_mathacl(pAlphaBeta->alpha, pSinCos->sin);
}
/******************************************************************************/

void MC_InverseParkTransform(MC_ALPHABETA_T *pAlphaBeta,
                             const MC_DQ_T *pDQ,
                             const MC_SINCOS_T *pSinCos)
{
    pAlphaBeta->alpha = _IQmpy_mathacl(pDQ->d, pSinCos->cos) -
            _IQmpy_mathacl(pDQ->q, pSinCos->sin);

    pAlphaBeta->beta = _IQmpy_mathacl(pDQ->d, pSinCos->sin) +
            _IQmpy_mathacl(pDQ->q, pSinCos->cos);
}
/******************************************************************************/
void MC_SinCos(MC_SINCOS_T *pSinCos, const int32_t angle)
{
    sinCosMATHACLCompute(angle);
    pSinCos->cos = readMATHACLCos();
    pSinCos->sin = readMATHACLSin();
}
/******************************************************************************/

void MC_SpaceVectorGen(MC_SVM_T *pSVM, MC_ABC_T *pMabc)
{
    if(pSVM->svmGen == SVM_DISCONTINUOUS)
    {
        pMabc->a = _IQsat(pMabc->a, _IQ(1.0), _IQ(-1.0));
        pMabc->b = _IQsat(pMabc->b, _IQ(1.0), _IQ(-1.0));
        pMabc->c = _IQsat(pMabc->c, _IQ(1.0), _IQ(-1.0));

    }

    int32_t maxABC = pMabc->a, minABC = pMabc->a, zeroSeq, maxDuty;
    MC_ABC_T mabcSVM;

    pSVM->maxDutyPhase = PHASE_A;
    pSVM->minDutyPhase = PHASE_A;

    /* Max determination */
    if(pMabc->b > maxABC)
    {
        maxABC = pMabc->b;
        pSVM->maxDutyPhase = PHASE_B;
    }
    if(pMabc->c > maxABC)
    {
        maxABC = pMabc->c;
        pSVM->maxDutyPhase = PHASE_C;
    }

    /* Min Determination */
    if(pMabc->b < minABC)
    {
        minABC = pMabc->b;
        pSVM->minDutyPhase = PHASE_B;
    }
    if(pMabc->c < minABC)
    {
        minABC = pMabc->c;
        pSVM->minDutyPhase = PHASE_C;
    }

    if(pSVM->svmGen == SVM_CONTINUOUS)
    {

        zeroSeq = (maxABC + minABC) >> 1;

        mabcSVM.a = _IQmpy_mathacl((pMabc->a - zeroSeq), ONE_BY_SQRT_THREE);
        mabcSVM.b = _IQmpy_mathacl((pMabc->b - zeroSeq), ONE_BY_SQRT_THREE);
        mabcSVM.c = _IQmpy_mathacl((pMabc->c - zeroSeq), ONE_BY_SQRT_THREE);

        pSVM->Dabc.a = ONE_BY_TWO + (mabcSVM.a);
        pSVM->Dabc.b = ONE_BY_TWO + (mabcSVM.b);
        pSVM->Dabc.c = ONE_BY_TWO + (mabcSVM.c);

        maxDuty = _IQ(1.0) - pSVM->minDuty;

        pSVM->Dabc.a = _IQsat(pSVM->Dabc.a, maxDuty, pSVM->minDuty);
        pSVM->Dabc.b = _IQsat(pSVM->Dabc.b, maxDuty, pSVM->minDuty);
        pSVM->Dabc.c = _IQsat(pSVM->Dabc.c, maxDuty, pSVM->minDuty);

    }

    else if(pSVM->svmGen == SVM_DISCONTINUOUS)
    {
        pSVM->Dabc.a = _IQmpy_mathacl((pMabc->a - minABC), ONE_BY_SQRT_THREE);
        pSVM->Dabc.b = _IQmpy_mathacl((pMabc->b - minABC), ONE_BY_SQRT_THREE);
        pSVM->Dabc.c = _IQmpy_mathacl((pMabc->c - minABC), ONE_BY_SQRT_THREE);

        maxDuty = _IQ(1.0) - pSVM->minDuty;

        pSVM->Dabc.a = _IQsat(pSVM->Dabc.a, maxDuty, pSVM->minDutyDisCont);
        pSVM->Dabc.b = _IQsat(pSVM->Dabc.b, maxDuty, pSVM->minDutyDisCont);
        pSVM->Dabc.c = _IQsat(pSVM->Dabc.c, maxDuty, pSVM->minDutyDisCont);

    }

    /* Sector Determination */
    if(pSVM->maxDutyPhase == PHASE_A)
    {
        if(pSVM->minDutyPhase == PHASE_B)
        {
            pSVM->voltageSector = SECTOR_6;
        }
        else
        {
            pSVM->voltageSector = SECTOR_1;
        }
    }

    if(pSVM->maxDutyPhase == PHASE_B)
    {
        if(pSVM->minDutyPhase == PHASE_A)
        {
            pSVM->voltageSector = SECTOR_3;
        }
        else
        {
            pSVM->voltageSector = SECTOR_2;
        }
    }

    if(pSVM->maxDutyPhase == PHASE_C)
    {
        if(pSVM->minDutyPhase == PHASE_A)
        {
            pSVM->voltageSector = SECTOR_4;
        }
        else
        {
            pSVM->voltageSector = SECTOR_5;
        }
    }

/* Toggle GPIO if FG output is Enabled in GateDriver.h*/
#ifdef FG_OUTPUT_ENABLE
    /* Toggle GPIO Every Sector to generate the FG Signal out */
    if((pSVM->voltageSector ==  SECTOR_3) && (pSVM->prevVoltageSector != SECTOR_3))
    {
        pSVM->sectorCount = 1;
    }
    else if((pSVM->sectorCount > 0) && (pSVM->voltageSector ==  SECTOR_3))
    {
        pSVM->sectorCount++;
        if(pSVM->sectorCount >= SECTOR_DEGLITCH_COUNT)
        {
            HAL_FGToggle();
            pSVM->sectorCount = 0;
        }
    }
    else
    {
        pSVM->sectorCount = 0;
    }
#endif

    pSVM->prevPrevVoltageSector = pSVM->prevVoltageSector;
    pSVM->prevVoltageSector = pSVM->voltageSector;
}

void MC_AngleSectorGen(MC_SVM_T *pSVM,
                       const MC_SINCOS_T *pSinCos,
                       const int32_t velocity)
{
    if(pSinCos->sin >= 0)
    {
        /* Rotor between 0deg and 180deg */
        if((pSinCos->cos >= _IQ(0.5)) &&
                ((velocity > 0 && pSVM->angleSector == SECTOR_6) ||
                        (velocity < 0 && pSVM->angleSector == SECTOR_2) ||
                        pSVM->angleSector == SECTOR_INVALID))
        {
            pSVM->angleSector = SECTOR_1;
        }
        else if((pSinCos->cos <= _IQ(-0.5)) &&
                ((velocity > 0 && pSVM->angleSector == SECTOR_2) ||
                        (velocity < 0 && pSVM->angleSector == SECTOR_4) ||
                        pSVM->angleSector == SECTOR_INVALID))
        {
            pSVM->angleSector = SECTOR_3;
        }
        else if((velocity > 0 && pSVM->angleSector == SECTOR_1) ||
                (velocity < 0 && pSVM->angleSector == SECTOR_3) ||
                pSVM->angleSector == SECTOR_INVALID)
        {
            pSVM->angleSector = SECTOR_2;
        }
    }
    else
    {
        /* Rotor between 180deg and 360deg */
        if((pSinCos->cos >= _IQ(0.5)) &&
                ((velocity > 0 && pSVM->angleSector == SECTOR_5) ||
                        (velocity < 0 && pSVM->angleSector == SECTOR_1) ||
                        pSVM->angleSector == SECTOR_INVALID))
        {
            pSVM->angleSector = SECTOR_6;
        }
        else if((pSinCos->cos <= _IQ(-0.5)) &&
                ((velocity > 0 && pSVM->angleSector == SECTOR_3) ||
                        (velocity < 0 && pSVM->angleSector == SECTOR_5) ||
                        pSVM->angleSector == SECTOR_INVALID))
        {
            pSVM->angleSector = SECTOR_4;
        }
        else if ((velocity > 0 && pSVM->angleSector == SECTOR_4) ||
                (velocity < 0 && pSVM->angleSector == SECTOR_6) ||
                pSVM->angleSector == SECTOR_INVALID)
        {
            pSVM->angleSector = SECTOR_5;
        }
    }
}

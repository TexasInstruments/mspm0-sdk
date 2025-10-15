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
 *  @file       mtpa.h
 *  @brief      mtpa Module
 *
 * 
 *  @anchor mtpa_h
 *  # Overview
 *
 *
 *  <hr>
 ******************************************************************************/


#ifndef FOC_INCLUDE_MTPA_H_
#define FOC_INCLUDE_MTPA_H_

#include "stdint.h"
#include "iqNum.h"
#include "main.h"

/*! @brief MTPA Working Structure */
typedef struct
{
    /*! MTPA Inputs */

    const int32_t
    /*! Inverse BEMF Constant */
        *pInvKe;

    const int16_t
    /*! Inverse BEMF Constant scaling */
        *pInvKeScaling;

    const int32_t
    /*! Q-axis current reference */
        *pIq;

    const int32_t
    /*! Minimum MTPA Current Reference */
        *pidRefMin;

    /*! MTPA Parameters for Processing */

    IQ_NUM_T
    /*! (Ld-Lq/2) normalized to speed and current base */
        L1;

    int32_t
    /*! MTPA Current Reference */
        idRef,
        /*! Filtered value of MTPA Current ref*/
        idRefFilt,
        /*! MTPA Current Reference Filter Constant */
        idFiltConst;

}MTPA_T;

/**
 * @brief     MTPA Init
 * @param[in] *pMtpa Input Pointer
 */
void mtpaInit(MTPA_T *pMtpa);
/**
 * @brief    MTPA Run
 * @param[in] *pMtpa Input Pointer
 */
void mtpaRun(MTPA_T *pMtpa);

#endif /* FOC_INCLUDE_MTPA_H_ */

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
 *  @file       fluxWeak.h
 *  @brief      fluxWeak Module
 *
 * 
 *  @anchor fluxWeak_h
 *  # Overview
 *
 *
 *  <hr>
 ******************************************************************************/


#ifndef FOC_INCLUDE_FLUXWEAK_H_
#define FOC_INCLUDE_FLUXWEAK_H_

#include "mcLib.h"
#include "motor.h"
#include "pi.h"
#include "main.h"

/*! @brief Flux Weakening Working Structure */
typedef struct
{

    /* Flux Weakening parameters for processing */

    int32_t
    /*! Reference for PI */
        mSqrRef,
    /*! Flux weakening current reference */
        idRef;

    PI_T
    /*! PI Structure */
        piFluxWeak;

}FLUX_WEAK_T;


/**
 * @brief     Flux weakening  Init
 * @param[in] *pFluxWeak Input Pointer
 */
void fluxWeakInit(FLUX_WEAK_T *pFluxWeak);

/**
 * @brief     Flux weakening  Run
 * @param[in] *pFluxWeak Input Pointer
 */
void fluxWeakRun(FLUX_WEAK_T *pFluxWeak);

#endif /* FOC_INCLUDE_FLUXWEAK_H_ */

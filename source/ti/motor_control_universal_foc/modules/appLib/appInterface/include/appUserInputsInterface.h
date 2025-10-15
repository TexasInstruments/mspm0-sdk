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
 *  @file       appUserInputsInterface.h
 *  @brief      Application Inputs Interface Module
 *
 * 
 *  @anchor appUserInputsInterface
 *  # Overview
 *
 *  defines user input Interface structure
 *
 *  <hr>
 ******************************************************************************/

#ifndef APPUSERINPUTSINTERFACE_H
#define APPUSERINPUTSINTERFACE_H

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! @brief User Input Interface structure */
typedef struct
{
    /* Contains User Input Derived Parameters used in Algorithm */
    float
        /*! Peak Phase Current Readable */
        phaseCurrentBase,
        /*! Motor Inductance Base */ 
        motorImpedanceBase,
        /*! ADC Sampling Frequency in kHz */
        adcSamplingFrequencyKHz,
        /*! ADC Sampling Rate in sec */
        adcSamplingRate,
        /*! Proportional - Current */
        kpCurrent,
        /*! Integral - Current */    
        kiCurrent,
        /*! Over Current Fault Base */
        overCurrentBase,
        /*! FSMO Real value */
        FSMO,
        /*! GSM real Value */
        GSMO;
    uint32_t
        /*! Inductance factor */
        L0MicroHenry,
        /*! Inductance factor */
        negL1MicroHenry;

}USER_INPUTS_INTERFACE_T;

#ifdef __cplusplus
}
#endif
#endif /* APPUSERINPUTSINTERFACE_H */

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
 *  @file       iqNum.h
 *  @brief      IQ Numeric Module
 *
 * 
 *  @anchor iqNum_h
 *  # Overview
 *
 *  defines for IQ
 *
 *  <hr>
 ******************************************************************************/

#ifndef IQNUM_H
#define IQNUM_H

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! @brief IQ_NUM structure */
typedef struct
{
    
    /*! Scaled number */
    int32_t
        scaledNum;

    /*! IQ scaling */
    int16_t
        iqScaling;
}IQ_NUM_T;

/**
 * @brief     Float to IQ conversion
 * @param[in]  pIqNum Pointer to IQ value
 * @param[in]  setValue float value
 */     
void float2IQ(IQ_NUM_T *pIqNum, const float setValue);

/**
 * @brief     Float to IQ conversion with scaling
 * @param[in]  pIqxNum Pointer to IQ value
 * @param[in]  setValue float value
 * @param[in]  iqScaling scaling factor
 */       
void float2IQx(int32_t *pIqxNum, const float setValue, const int16_t iqScaling);

#ifdef __cplusplus
}
#endif
#endif /* IQNUM_H */

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
 *  @file       _IQNdiv_mathacl.h
 *  @brief      Functions to divide two values of IQN type.
 *
 *  <hr>
 ******************************************************************************/

#ifndef IQNDIV_MATHACL
#define IQNDIV_MATHACL
#include <stdint.h>

#include <ti/devices/msp/msp.h>

/**
 * @brief Divide two values of IQN type, using MathACL
 *
 * @param iqNInput1       IQN type value numerator to be divided.
 * @param iqNInput2       IQN type value denominator to divide by.
 * @param q_value         IQ format.
 *
 * @return                IQN type result of the multiplication.
 */
__STATIC_INLINE int_fast32_t __IQNdiv_mathacl(int_fast32_t iqNInput1, int_fast32_t iqNInput2, const int8_t q_value)
{
    /* write control */
    MATHACL->CTL = 4 | (q_value<<8) | (1 << 5);
    /* write operands to HWA. OP2 = divisor, OP1 = dividend */
    MATHACL->OP2 = iqNInput2;
    /* trigger is write to OP1 */
    MATHACL->OP1 = iqNInput1;
    /* read quotient and remainder */
    return MATHACL->RES1;
}
#endif

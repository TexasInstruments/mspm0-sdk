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

#include <stdint.h>

#include <ti/devices/msp/msp.h>

/*!
 * @brief Specifies inverse square root operation type.
 */
#define TYPE_ISQRT   (0)
/*!
 * @brief Specifies square root operation type.
 */
#define TYPE_SQRT    (1)
/*!
 * @brief Specifies magnitude operation type.
 */
#define TYPE_MAG     (2)
/*!
 * @brief Specifies inverse magnitude operation type.
 */
#define TYPE_IMAG    (3)

/**
 * @brief Calculate square root of an  IQN input, using MathACL.
 *
 * @param iqNInputX         IQN type input.
 * @param q_value           IQ format.
 *
 * @return                  IQN type result of the square root operation.
 */
__STATIC_INLINE int_fast32_t __IQNsqrt_mathacl(int_fast32_t iqNInputX, const int8_t q_value)
{
    /* check sign of input */
    if (iqNInputX <= 0) {
        return 0;
    }

    /* Scale factor computation:
     * output: IQ30 format value whose square root is to be computed by MATHACL
     * scale_factor: (n) where the input has been divided by 2^(2n) to render IQ30
     * value in the range (1,2). 
     */
    uint32_t input, output;
    uint8_t scale_factor;
    uint8_t n = 0;

    input = iqNInputX;
    /* check input is withing 32-bit boundaries */
    if (input & 0x80000000)
    {
        scale_factor = 0;
        output = input;
    }
    else
    {
        n = 0;
        /* check while input != IQ30(1.0) */
        while ((input & 0x40000000) == 0) {
            n++;
            /* multiply by 2 until reaching IQ30 [1.0,2.0 range] */
            input <<= 1;
        }
        /* 
         * Scale factor: take into account the shift from q_value to IQ30, the remaining value 
         * is the scale factor such that scaled number = (nonscaled number)^(2^scale_factor)
         */
        scale_factor = (30-q_value)-n;
        output = input;
    }
    /* SQRT MATHACL Operation
     * write control 
     * CTL parameters are: sqrt operation | number of iterations | scale factor
     */
    MATHACL->CTL = 5 | (31 << 24) | (scale_factor << 16);
    /* write operands to HWA
     * write to OP1 is the trigger
     */
    MATHACL->OP1 = output;
    /* read sqrt 
     * shift output from IQ16 to q_value 
     */
    if(q_value>16){
        return (uint_fast32_t)MATHACL->RES1 << (q_value - 16);
    }
    else{
        return (uint_fast32_t)MATHACL->RES1 >> (16 - q_value);
    }
}

/**
 * @brief Calculate square root of an IQ31 input, using MathACL.
 *
 * @param a                 IQ31 type input.
 *
 * @return                  IQ31 type result of the square root operation.
 */
int32_t _IQ31sqrt_mathacl(int32_t a)
{
    return __IQNsqrt_mathacl(a, 31);
}
/**
 * @brief Calculate square root of an IQ30 input, using MathACL.
 *
 * @param a                 IQ30 type input.
 *
 * @return                  IQ30 type result of the square root operation.
 */
int32_t _IQ30sqrt_mathacl(int32_t a)
{
    return __IQNsqrt_mathacl(a, 30);
}
/**
 * @brief Calculate square root of an IQ29 input, using MathACL.
 *
 * @param a                 IQ29 type input.
 *
 * @return                  IQ29 type result of the square root operation.
 */
int32_t _IQ29sqrt_mathacl(int32_t a)
{
    return __IQNsqrt_mathacl(a, 29);
}
/**
 * @brief Calculate square root of an IQ28 input, using MathACL.
 *
 * @param a                 IQ28 type input.
 *
 * @return                  IQ28 type result of the square root operation.
 */
int32_t _IQ28sqrt_mathacl(int32_t a)
{
    return __IQNsqrt_mathacl(a, 28);
}
/**
 * @brief Calculate square root of an IQ27 input, using MathACL.
 *
 * @param a                 IQ27 type input.
 *
 * @return                  IQ27 type result of the square root operation.
 */
int32_t _IQ27sqrt_mathacl(int32_t a)
{
    return __IQNsqrt_mathacl(a, 27);
}
/**
 * @brief Calculate square root of an IQ26 input, using MathACL.
 *
 * @param a                 IQ26 type input.
 *
 * @return                  IQ26 type result of the square root operation.
 */
int32_t _IQ26sqrt_mathacl(int32_t a)
{
    return __IQNsqrt_mathacl(a, 26);
}
/**
 * @brief Calculate square root of an IQ25 input, using MathACL.
 *
 * @param a                 IQ25 type input.
 *
 * @return                  IQ25 type result of the square root operation.
 */
int32_t _IQ25sqrt_mathacl(int32_t a)
{
    return __IQNsqrt_mathacl(a, 25);
}
/**
 * @brief Calculate square root of an IQ24 input, using MathACL.
 *
 * @param a                 IQ24 type input.
 *
 * @return                  IQ24 type result of the square root operation.
 */
int32_t _IQ24sqrt_mathacl(int32_t a)
{
    return __IQNsqrt_mathacl(a, 24);
}
/**
 * @brief Calculate square root of an IQ23 input, using MathACL.
 *
 * @param a                 IQ23 type input.
 *
 * @return                  IQ23 type result of the square root operation.
 */
int32_t _IQ23sqrt_mathacl(int32_t a)
{
    return __IQNsqrt_mathacl(a, 23);
}
/**
 * @brief Calculate square root of an IQ22 input, using MathACL.
 *
 * @param a                 IQ22 type input.
 *
 * @return                  IQ22 type result of the square root operation.
 */
int32_t _IQ22sqrt_mathacl(int32_t a)
{
    return __IQNsqrt_mathacl(a, 22);
}
/**
 * @brief Calculate square root of an IQ21 input, using MathACL.
 *
 * @param a                 IQ21 type input.
 *
 * @return                  IQ21 type result of the square root operation.
 */
int32_t _IQ21sqrt_mathacl(int32_t a)
{
    return __IQNsqrt_mathacl(a, 21);
}
/**
 * @brief Calculate square root of an IQ20 input, using MathACL.
 *
 * @param a                 IQ20 type input.
 *
 * @return                  IQ20 type result of the square root operation.
 */
int32_t _IQ20sqrt_mathacl(int32_t a)
{
    return __IQNsqrt_mathacl(a, 20);
}
/**
 * @brief Calculate square root of an IQ19 input, using MathACL.
 *
 * @param a                 IQ19 type input.
 *
 * @return                  IQ19 type result of the square root operation.
 */
int32_t _IQ19sqrt_mathacl(int32_t a)
{
    return __IQNsqrt_mathacl(a, 19);
}
/**
 * @brief Calculate square root of an IQ18 input, using MathACL.
 *
 * @param a                 IQ18 type input.
 *
 * @return                  IQ18 type result of the square root operation.
 */
int32_t _IQ18sqrt_mathacl(int32_t a)
{
    return __IQNsqrt_mathacl(a, 18);
}
/**
 * @brief Calculate square root of an IQ17 input, using MathACL.
 *
 * @param a                 IQ17 type input.
 *
 * @return                  IQ17 type result of the square root operation.
 */
int32_t _IQ17sqrt_mathacl(int32_t a)
{
    return __IQNsqrt_mathacl(a, 17);
}
/**
 * @brief Calculate square root of an IQ16 input, using MathACL.
 *
 * @param a                 IQ16 type input.
 *
 * @return                  IQ16 type result of the square root operation.
 */
int32_t _IQ16sqrt_mathacl(int32_t a)
{
    return __IQNsqrt_mathacl(a, 16);
}
/**
 * @brief Calculate square root of an IQ15 input, using MathACL.
 *
 * @param a                 IQ15 type input.
 *
 * @return                  IQ15 type result of the square root operation.
 */
int32_t _IQ15sqrt_mathacl(int32_t a)
{
    return __IQNsqrt_mathacl(a, 15);
}
/**
 * @brief Calculate square root of an IQ14 input, using MathACL.
 *
 * @param a                 IQ14 type input.
 *
 * @return                  IQ14 type result of the square root operation.
 */
int32_t _IQ14sqrt_mathacl(int32_t a)
{
    return __IQNsqrt_mathacl(a, 14);
}
/**
 * @brief Calculate square root of an IQ13 input, using MathACL.
 *
 * @param a                 IQ13 type input.
 *
 * @return                  IQ13 type result of the square root operation.
 */
int32_t _IQ13sqrt_mathacl(int32_t a)
{
    return __IQNsqrt_mathacl(a, 13);
}
/**
 * @brief Calculate square root of an IQ12 input, using MathACL.
 *
 * @param a                 IQ12 type input.
 *
 * @return                  IQ12 type result of the square root operation.
 */
int32_t _IQ12sqrt_mathacl(int32_t a)
{
    return __IQNsqrt_mathacl(a, 12);
}
/**
 * @brief Calculate square root of an IQ11 input, using MathACL.
 *
 * @param a                 IQ11 type input.
 *
 * @return                  IQ11 type result of the square root operation.
 */
int32_t _IQ11sqrt_mathacl(int32_t a)
{
    return __IQNsqrt_mathacl(a, 11);
}
/**
 * @brief Calculate square root of an IQ10 input, using MathACL.
 *
 * @param a                 IQ10 type input.
 *
 * @return                  IQ10 type result of the square root operation.
 */
int32_t _IQ10sqrt_mathacl(int32_t a)
{
    return __IQNsqrt_mathacl(a, 10);
}
/**
 * @brief Calculate square root of an IQ9 input, using MathACL.
 *
 * @param a                 IQ9 type input.
 *
 * @return                  IQ9 type result of the square root operation.
 */
int32_t _IQ9sqrt_mathacl(int32_t a)
{
    return __IQNsqrt_mathacl(a, 9);
}
/**
 * @brief Calculate square root of an IQ8 input, using MathACL.
 *
 * @param a                 IQ8 type input.
 *
 * @return                  IQ8 type result of the square root operation.
 */
int32_t _IQ8sqrt_mathacl(int32_t a)
{
    return __IQNsqrt_mathacl(a, 8);
}
/**
 * @brief Calculate square root of an IQ7 input, using MathACL.
 *
 * @param a                 IQ7 type input.
 *
 * @return                  IQ7 type result of the square root operation.
 */
int32_t _IQ7sqrt_mathacl(int32_t a)
{
    return __IQNsqrt_mathacl(a, 7);
}
/**
 * @brief Calculate square root of an IQ6 input, using MathACL.
 *
 * @param a                 IQ6 type input.
 *
 * @return                  IQ6 type result of the square root operation.
 */
int32_t _IQ6sqrt_mathacl(int32_t a)
{
    return __IQNsqrt_mathacl(a, 6);
}
/**
 * @brief Calculate square root of an IQ5 input, using MathACL.
 *
 * @param a                 IQ5 type input.
 *
 * @return                  IQ5 type result of the square root operation.
 */
int32_t _IQ5sqrt_mathacl(int32_t a)
{
    return __IQNsqrt_mathacl(a, 5);
}
/**
 * @brief Calculate square root of an IQ4 input, using MathACL.
 *
 * @param a                 IQ4 type input.
 *
 * @return                  IQ4 type result of the square root operation.
 */
int32_t _IQ4sqrt_mathacl(int32_t a)
{
    return __IQNsqrt_mathacl(a, 4);
}
/**
 * @brief Calculate square root of an IQ3 input, using MathACL.
 *
 * @param a                 IQ3 type input.
 *
 * @return                  IQ3 type result of the square root operation.
 */
int32_t _IQ3sqrt_mathacl(int32_t a)
{
    return __IQNsqrt_mathacl(a, 3);
}
/**
 * @brief Calculate square root of an IQ2 input, using MathACL.
 *
 * @param a                 IQ2 type input.
 *
 * @return                  IQ2 type result of the square root operation.
 */
int32_t _IQ2sqrt_mathacl(int32_t a)
{
    return __IQNsqrt_mathacl(a, 2);
}
/**
 * @brief Calculate square root of an IQ1 input, using MathACL.
 *
 * @param a                 IQ1 type input.
 *
 * @return                  IQ1 type result of the square root operation.
 */
int32_t _IQ1sqrt_mathacl(int32_t a)
{
    return __IQNsqrt_mathacl(a, 1);
}
/**
 * @brief Calculate square root of an IQ0 input, using MathACL.
 *
 * @param a                 IQ0 type input.
 *
 * @return                  IQ0 type result of the square root operation.
 */
int32_t _IQ0sqrt_mathacl(int32_t a)
{
    return __IQNsqrt_mathacl(a, 0);
}

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

#include "iqTrig.h"
#include "appDefs.h"

/*! @brief Sine lookup table */
const int32_t sineTbl[LOOKUP_POINTS] =  {_IQ(0),
                                         _IQ(0.104528463267653),
                                         _IQ(0.207911690817759),
                                         _IQ(0.309016994374947),
                                         _IQ(0.4067366430758),
                                         _IQ(0.5),
                                         _IQ(0.587785252292473),
                                         _IQ(0.669130606358858),
                                         _IQ(0.743144825477394),
                                         _IQ(0.809016994374947),
                                         _IQ(0.866025403784439),
                                         _IQ(0.913545457642601),
                                         _IQ(0.951056516295154),
                                         _IQ(0.978147600733806),
                                         _IQ(0.994521895368273),
                                         _IQ(1.0) };

int32_t sine_LowPriority(int32_t orig_angle)
{
    return cosine_LowPriority(_IQ(0.25) - orig_angle);
}

int32_t cosine_LowPriority(int32_t orig_angle)
{
    int32_t
        angle,
        lookupTableAngle,
        y_low_index,
        y_high_index,
        angleError,
        y_interp,
        temp1,
        temp2;

    int16_t
        quadrant,
        lowIndex,
        highIndex;

    angle = (orig_angle) & 0x07FFFFFF;

    if(angle >= 0 && angle < _IQ(0.25))
    {
        quadrant = 0;
        lookupTableAngle = _IQ(0.25) - angle;
    }
    else if(angle >= _IQ(0.25) && angle < _IQ(0.5))
    {
        quadrant = 1;
        lookupTableAngle = angle - _IQ(0.25);
    }
    else if(angle >= _IQ(0.5) && angle < _IQ(0.75))
    {
        quadrant = 2;
        lookupTableAngle = _IQ(0.75) - angle;
    }
    else
    {
        quadrant = 3;
        lookupTableAngle = angle - _IQ(0.75);
    }

    lowIndex = _IQmpy(lookupTableAngle , INV_DELX_LOOKUP);
    highIndex = lowIndex + 1;

    if(lowIndex >= (LOOKUP_POINTS - 1))
    {
        lowIndex = LOOKUP_POINTS - 1;
    }

    if(highIndex >= (LOOKUP_POINTS - 1))
    {
        highIndex = LOOKUP_POINTS - 1;
    }

    y_low_index = sineTbl[lowIndex];
    y_high_index = sineTbl[highIndex];
    angleError = lookupTableAngle - DELX_LOOKUP * lowIndex;

    if(lowIndex != highIndex)
    {
        temp1 = _IQmpy((y_high_index - y_low_index), angleError);
        temp2 = (temp1 * INV_DELX_LOOKUP);

        y_interp = y_low_index + temp2;
    }
    else
    {
        y_interp = y_low_index;
    }

    if(quadrant == 1 || quadrant == 2)
    {
        y_interp = -y_interp;
    }

    return y_interp;
}

/*! @brief Workaround to fix error see in result for 0xC0000000 and 0x80000000 OP1 input
The workaround is to make the LSB as 1 for all inputs with doesn't include the before mentioned inputs
this fix is selected as it doesn't consume much cycles */
#define _IQTRIG_MATHACL_SINCOS_BUG_WORKAROUND_

void sinCosMATHACLCompute(int_fast32_t orig_angle)
{
    int_fast32_t iq31input, iqNInput;

    iqNInput = (orig_angle) & 0x07FFFFFF;

    /* multiply by 2 for MathACL scaling and shift to IQ31 for sin/cos calculation */
    iq31input = (uint_fast32_t)iqNInput << (32 - GLOBAL_IQ);

#ifdef _IQTRIG_MATHACL_SINCOS_BUG_WORKAROUND_
    iq31input |= 0x1;
#endif
    
    /*
     * write control
     * operation = sincos, iterations = 31
     */
    MATHACL->CTL = 1 | (31 << 24);
    /* write operand to HWA */
    MATHACL->OP1 = iq31input;

}

int_fast32_t sine(int_fast32_t orig_angle)
{
    sinCosMATHACLCompute(orig_angle);

    return readMATHACLSin();
}

int_fast32_t cosine(int_fast32_t orig_angle)
{
    sinCosMATHACLCompute(orig_angle);

    return readMATHACLCos();
}

int_fast32_t arcTan2(int_fast32_t iqNInputX, int_fast32_t iqNInputY)
{
    int_fast32_t res, abs_max, XMax, YMax;
    int_fast32_t iqNnormX, iqNnormY, iq31normX, iq31normY;
    /* ATAN2 Operation with MatchACL requires X,Y input values to be IQ31.
     * Therefore, we need to normalize the input values.
     */

    /* Normalize input values by using the largest abs max input value */
    /* Code for _IQXabs is the same for all Q values */
    YMax = _IQabs(iqNInputY);
    XMax = _IQabs(iqNInputX);
    if(YMax > XMax)
    {
        abs_max = YMax;
    }
    else
    {
        abs_max = XMax;
    }

    /* Both inputs are 0 */
    if(abs_max == 0)
    {
        return 0;
    }

    /* IQ31 doesn't support 1.0. Therefore, we need to ensure the normalized
     * values are not equal to 1 but rather slightly below 1.0.
     */

    /* Check to see if abs_max is equal to the max value
     * represented by the specified Q value (0x7FFFFFFFF) being represented.
     * This will determine which approach is taken to ensure abs_max is > than
     * the abs value of either inputs when normalization is performed.
     */
    if(abs_max == 0x7FFFFFFF)
    {
        /* Scale down the inputs slightly so we ensure that abs_max is slightly
         * larger than the abs value of the inputs.
         */
        iqNInputX = iqNInputX - (iqNInputX >> GLOBAL_IQ);
        iqNInputY = iqNInputY - (iqNInputY >> GLOBAL_IQ);
    }
    else
    {
        /* The decimal value 1 is the smallest positive value for a given IQ.
         * So by adding this to our variable we are using to normalize we
         * ensure the resulting normalized values are < 1.0.
         */
        abs_max += 1;
    }

    /* Normalize Inputs */
    iqNnormX = _IQdiv_mathacl(iqNInputX,abs_max);
    iqNnormY = _IQdiv_mathacl(iqNInputY,abs_max);

    /* Shift from IQX to IQ31 which is required for MathACL ATAN2 operation */
    iq31normX = (uint_fast32_t)iqNnormX << (31-GLOBAL_IQ);
    iq31normY = (uint_fast32_t)iqNnormY << (31-GLOBAL_IQ);

    /* MathACL ATAN2 Operation */
    MATHACL->CTL = 2 | (31 << 24);
    /* write operands to HWA */
    MATHACL->OP2 = iq31normY;
    /* write to OP1 is the trigger */
    MATHACL->OP1 = iq31normX;

    /* read atan2 */
    /* change the base to 2pi also Shift to q_value type also divide by 2 to scale to correct format */
    res = ((uint_fast32_t)MATHACL->RES1>> (32 - GLOBAL_IQ));
    return res;
}

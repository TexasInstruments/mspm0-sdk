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

int32_t sine(int32_t orig_angle)
{
    return cosine(_IQ(0.25) - orig_angle);
}

int32_t cosine(int32_t orig_angle)
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

    lowIndex = _IQmpy_mathacl(lookupTableAngle , INV_DELX_LOOKUP);
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
        temp1 = _IQmpy_mathacl((y_high_index - y_low_index), angleError);
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

int32_t arcTan2(int32_t cosVal, int32_t sinVal, int32_t mag)
{
    int32_t
        sinValAbs,
        numerator,
        quadAngle,
        angle,
        count,
        x_low_index,
        x_high_index,
        y_low_index,
        y_high_index,
        lowIndex = 0,
        highIndex;

    sinValAbs = _IQdiv_mathacl(_IQabs(sinVal), mag);

    for(count = 0; count < LOOKUP_POINTS; count++)
    {
        if(sinValAbs >= sineTbl[count])
        {
            lowIndex = count;
        }
    }

    highIndex = lowIndex + 1;

    if(lowIndex >= (LOOKUP_POINTS - 1))
    {
        lowIndex = LOOKUP_POINTS - 1;
    }

    if(highIndex >= (LOOKUP_POINTS - 1))
    {
        highIndex = LOOKUP_POINTS - 1;
    }

    x_low_index = lowIndex * DELX_LOOKUP;
    x_high_index = highIndex * DELX_LOOKUP;

    y_low_index = sineTbl[lowIndex];
    y_high_index = sineTbl[highIndex];

    numerator = _IQmpy_mathacl(sinValAbs, DELX_LOOKUP) -
                _IQmpy_mathacl(y_low_index, x_high_index) +
                _IQmpy_mathacl(y_high_index, x_low_index);

    if(y_high_index == y_low_index)
    {
        quadAngle = _IQ(0.25);
    }
    else
    {
        quadAngle = _IQdiv_mathacl(numerator, (y_high_index - y_low_index));
    }

    if(cosVal > 0)
    {
        if(sinVal > 0)
        {
            angle = quadAngle;
        }
        else
        {
            angle = _IQ(1.0) - quadAngle;
        }
    }
    else
    {
        if(sinVal > 0)
        {
            angle = _IQ(0.5) - quadAngle;
        }
        else
        {
            angle = quadAngle - _IQ(0.5);
        }
    }
    return angle;
}

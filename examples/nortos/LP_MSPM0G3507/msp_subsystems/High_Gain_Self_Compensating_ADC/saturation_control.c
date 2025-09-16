/*
 * Copyright (c) 2021, Texas Instruments Incorporated
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
//////////////////////////////////////////////////////////////////////////////

#include "saturation_control.h"
#include "ti_msp_dl_config.h"
#include "customizable_features.h"

/**** Local Variables ********************************************************/
int8_t delta_v_out2;      
uint16_t v_dac2_new;

/**** Global Extern Variable ********************************************************/
extern uint32_t gDAC8_1_DATA;

/**** Functions **************************************************************/

/*! \brief Calculate the DAC8 value of 2nd stage OPA if output voltage out of range.
 *
 * Vdac = Vdac - [(Vout - V desired) / (G + 1)]
 *
 *  \return none
 */
void
saturation_control (uint16_t v_out2)
{
#   define HALF_FSR 128

    if ((v_out2 < OPA1_output_min)||(v_out2 > OPA1_output_max))
    {
        /* Convert OPA1 output to 8 bit */
        v_out2 = v_out2 >> 4;      
        /* Difference between OPA1 output and desired */
        delta_v_out2 = v_out2 - HALF_FSR;          

        /* Vdac = Vdac - [(Vout - V desired) / (G + 1)] */
        v_dac2_new = gDAC8_1_DATA;
        v_dac2_new -= (delta_v_out2 /(OPA1_Gain+1))>>SAT_SHIFT;

        /* Limits based on ADC's range */
        if (v_dac2_new>253)
            v_dac2_new = 253;
        else if (v_dac2_new <3)
            v_dac2_new = 3;

        gDAC8_1_DATA = v_dac2_new;
        DL_COMP_setDACCode0(COMP_1_INST, gDAC8_1_DATA);
        DL_COMP_enable(COMP_1_INST);
    }
}   /* saturation_control() */
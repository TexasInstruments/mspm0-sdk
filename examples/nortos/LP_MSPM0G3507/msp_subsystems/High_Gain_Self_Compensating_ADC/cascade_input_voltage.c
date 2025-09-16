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

#include "cascade_input_voltage.h"
#include "ti_msp_dl_config.h"
#include <stdint.h>
#include <stdbool.h>
#include "customizable_features.h"

/**** Global Variables ********************************************************/
uint16_t gOPA1_output;           
uint32_t gOPA0_output;

/**** Global Extern Variable ********************************************************/
extern uint32_t gDAC8_0_DATA;
extern uint32_t gDAC8_1_DATA;

/**** Function **************************************************************/

/*! \brief Calculates Cascaded Gained Input:  G1*G2*Vin1
 *
 *  (G1*G2*Vin1) = Vout2 + ((1+G1)*G2*Vdac1)*2^4 - ((1+G2)*Vdac2*2^4)
 *
 *  \return G1*G2*Vin1
 */
uint32_t cascade_input_voltage (uint32_t gOPA1_output)
{
    uint32_t result;
    /*
     * (G1*G2*Vin1/4096)*VDDA = (Vout2/4096)*VDDA + ((1+G1)*G2*Vdac1/256)*VDDA - ((1+G2)*Vdac2/256)*VDDA
     * (G1*G2*Vin1) = Vout2 + ((1+G1)*G2*Vdac1)*2^4 - ((1+G2)*Vdac2*2^4)
     *
     * for:
     *      G1 = OPA0_Gain
     *      G2 = OPA1_Gain
     */
    result = (gDAC8_0_DATA * 4) * ((OPA0_Gain+1) * OPA1_Gain );
    result -= (gDAC8_1_DATA * 4) * (OPA1_Gain + 1);
    result +=  gOPA1_output;

    return (result);
}   /* calc_cascade_input_voltage() */
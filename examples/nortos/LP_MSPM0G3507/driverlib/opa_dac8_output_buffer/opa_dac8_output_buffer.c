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

#include "ti_msp_dl_config.h"

/*
 * DAC8 Reference Voltage in mV
 *  Adjust this value according to the internal VREF configuration.
 */
#define COMP_0_INST_REF_VOLTAGE_mV (3300)
/*
 * DAC8 static output voltage in mV.
 *  Adjust output as needed.
 */
#define COMP_0_INST_DAC8_OUTPUT_VOLTAGE_mV (1000)

int main(void)
{
    uint32_t dacValue;

    SYSCFG_DL_init();

    /*
     * Set output voltage:
     *  DAC value (8-bits) = DesiredOutputVoltage x 255
     *                          -----------------------
     *                              ReferenceVoltage
     */
    dacValue = (COMP_0_INST_DAC8_OUTPUT_VOLTAGE_mV * 255) /
               COMP_0_INST_REF_VOLTAGE_mV;

    DL_COMP_setDACCode0(COMP_0_INST, dacValue);
    DL_COMP_enable(COMP_0_INST);

    DL_OPA_enable(OPA_0_INST);

    while (1) {
        __WFI();
    }
}
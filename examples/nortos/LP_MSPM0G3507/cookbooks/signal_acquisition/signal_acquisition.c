/*
 * Copyright (c) 2020, Texas Instruments Incorporated
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
 * This example demonstrates two sensor connections
 * sharing one OPA and ADC channel.
 */

/* Supported OPA Configurations */
enum OPA_Configurations
{
    eBuffer = 0,
    eNonInverting = 1,
};
enum OPA_Configurations OPAConfig = eBuffer;

/*
 * Declarations
 */
void OPA0_Buffer();
void OPA0_NonInverting();

/* Array to store ADC channel measurements */
volatile uint16_t ADC0_Result;
uint16_t ADC0_sensorOutput[2];
bool gADC0_Flag = false;

int main(void)
{
    SYSCFG_DL_init();

    /* Start with Buffer configuration */
    OPAConfig = eBuffer;
    OPA0_Buffer();

    NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);

    DL_ADC12_enableConversions(ADC12_0_INST);

    while (1) {

        /* Remain in low-power mode until ADC interrupt is serviced */
        if(!(gADC0_Flag))
            __WFI();

        if(gADC0_Flag == true)
        {
            /* Clear flag */
            gADC0_Flag = false;

            /*
             * Reconfigure OPA for next sensor
             */
            if(OPAConfig == eBuffer)
            {
                ADC0_sensorOutput[eBuffer] = ADC0_Result;
                __BKPT(0); /* set a BKPT to examine result */

                /* Configure OPA as Non-Inverting */
                OPAConfig = eNonInverting;
                OPA0_NonInverting();

                /*
                 * Perform any post processing here
                 */
            }
            else
            {
                ADC0_sensorOutput[eNonInverting] = ADC0_Result;
                __BKPT(0); /* set a BKPT to examine result */

                /* Configure OPA as Buffer */
                OPAConfig = eBuffer;
                OPA0_Buffer();

                /*
                 * Perform any post processing here...
                 */
            }

        }
    }
}

void ADC12_0_INST_IRQHandler(void)
{
    uint8_t ui8Index = 0;

    switch (DL_ADC12_getPendingInterrupt(ADC12_0_INST)){

    /* Interrupt on last measurement (MEM3), then read results */
    case DL_ADC12_IIDX_MEM0_RESULT_LOADED:

        ADC0_Result = DL_ADC12_getMemResult(ADC12_0_INST, 0);

        gADC0_Flag = true;

        DL_ADC12_enableConversions(ADC12_0_INST);
        break;

    default:
        break;

    } 
}


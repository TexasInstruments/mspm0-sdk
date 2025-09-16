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

#include "dac_init.h"
#include "customizable_features.h"
#include "ti_msp_dl_config.h"
#include <stdint.h>
#include <stdbool.h>

/**** Global Variable Declarations **************************************/
uint32_t gDAC8_0_DATA;
uint32_t gDAC8_1_DATA;

/**** Global Variable Extern Declarations **************************************/
extern volatile bool gCheckADC_0;
extern volatile bool gCheckADC_1;

/**** Local Function Prototypes ***********************************************/
static inline void initial_DAC8_0(void);
static inline void initial_DAC8_1(void);

/**** Functions **************************************************************/
void
dac_init (void)
{
    initial_DAC8_0();
    initial_DAC8_1();
}   /* dac_init() */

/*! \brief Initial DAC8_0
 *
 *  1)Measure output voltage, ADC0.13.
 *  2)Update DAC8_0 value.
 *
 *  \return none
 */
static inline void
initial_DAC8_0 (void)
{
    uint32_t OPA0_output_value;
        /* Initial DAC8 value */
        gDAC8_0_DATA = 117;
        DL_COMP_setDACCode0(COMP_0_INST, gDAC8_0_DATA);
        DL_COMP_enable(COMP_0_INST);

        /* software delay for DAC signals to settle:  100ms */
        delay_cycles(3000000);  

        /* ADC Channel ADC0.13 (OPA0 output signal) */ 
        DL_ADC12_enableConversions(ADC12_0_INST);
        DL_ADC12_startConversion(ADC12_0_INST);
        DL_SYSCTL_setPowerPolicyRUN0SLEEP0();

        while (false == gCheckADC_0) {
            __WFE();
        }
        OPA0_output_value = DL_ADC12_getMemResult(ADC12_0_INST, DL_ADC12_MEM_IDX_0);

        while ((OPA0_output_value < init_OPA0_output_min )|| (OPA0_output_value > init_OPA0_output_max))
        {
            /* ADC Channel ADC0.13 (OPA0 output) */ 
            DL_ADC12_enableConversions(ADC12_0_INST);
            DL_ADC12_startConversion(ADC12_0_INST);
            DL_SYSCTL_setPowerPolicyRUN0SLEEP0();

           while (false == gCheckADC_0) {
                __WFE();
            }
            OPA0_output_value = DL_ADC12_getMemResult(ADC12_0_INST, DL_ADC12_MEM_IDX_0);
            gCheckADC_0 = false;

            if (OPA0_output_value < init_OPA0_output_min)
                gDAC8_0_DATA++;
            else if (OPA0_output_value > init_OPA0_output_max)
                gDAC8_0_DATA--;

            /* Set output voltage */ 
            DL_COMP_setDACCode0(COMP_0_INST, gDAC8_0_DATA);
            DL_COMP_enable(COMP_0_INST);

            /* software delay for DAC signals to settle:  10ms */
            delay_cycles(300000);     
        }
}   /*     initial_DAC8_0() */


/*! \brief Initial DAC8_1
 *
 *  1)Measure output voltage, ADC1.13.
 *  2)Update DAC8_1 value.
 *
 *  \return none
 */
static inline void
initial_DAC8_1 (void)
{
        uint32_t OPA1_output_value;
            /* DAC8.1 initial value calculation */
            gDAC8_1_DATA = 120;
            DL_COMP_setDACCode0(COMP_1_INST, gDAC8_1_DATA);
            DL_COMP_enable(COMP_1_INST);

            /* software delay:  100ms */
            delay_cycles(3000000);     

            /* ADC Channel ADC1.13 (OPA1 output) */ 
            DL_ADC12_enableConversions(ADC12_1_INST);
            DL_ADC12_startConversion(ADC12_1_INST);
            DL_SYSCTL_setPowerPolicyRUN0SLEEP0();

            while (false == gCheckADC_1) {
                   __WFE();
                }
            OPA1_output_value = DL_ADC12_getMemResult(ADC12_1_INST, DL_ADC12_MEM_IDX_0);

            while ((OPA1_output_value < init_OPA1_output_min )|| (OPA1_output_value > init_OPA1_output_max))
               {
                   /* ADC Channel ADC1.13 (OPA1 output signal) */ 
                   DL_ADC12_enableConversions(ADC12_1_INST);
                   DL_ADC12_startConversion(ADC12_1_INST);
                   DL_SYSCTL_setPowerPolicyRUN0SLEEP0();

                   while (false == gCheckADC_1) {
                       __WFE();
                   }
                   OPA1_output_value = DL_ADC12_getMemResult(ADC12_1_INST, DL_ADC12_MEM_IDX_0);
                   gCheckADC_1 = false;

                   if (OPA1_output_value < init_OPA1_output_min)
                       gDAC8_1_DATA++;
                   else if (OPA1_output_value > init_OPA1_output_max)
                       gDAC8_1_DATA--;

                   /* Set output voltage: */
                   DL_COMP_setDACCode0(COMP_1_INST, gDAC8_1_DATA);
                   DL_COMP_enable(COMP_1_INST);

                   /* software delay:  40ms */
                   delay_cycles(1000000);    
               }
}   /*     initial_DAC8_1() */
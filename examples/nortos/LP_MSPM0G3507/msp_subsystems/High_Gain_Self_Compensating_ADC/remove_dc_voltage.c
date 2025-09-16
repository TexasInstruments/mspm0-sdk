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

#include "remove_dc_voltage.h"
#include "ti_msp_dl_config.h"
#include "customizable_features.h"
#include <stdint.h>

/**** Global Variable Extern Declarations **************************************/
extern uint32_t gDAC8_0_DATA;  
extern volatile bool gCheckADC_0;
extern volatile bool gCheckADC_1;
extern uint16_t gOPA1_output;
extern uint32_t gOPA0_output;
/**********************************/

/**** Local Function Prototypes ***********************************************/
static inline void initial_DAC8_0(void);
static inline void calc_voltage(void);
static inline void ema_gained_input_voltage(void);
static inline void calc_dac_dc_voltage(void);

/**** Local Variables ********************************************************/

/* Calculated G1*Vin1(n) */
uint32_t gained_input_voltage_value;

/* y(n) <= EMA(G1*Vin1(n)) */
uint32_t ema_output_value_yn;    

/* Calculated DAC value for bias */
uint32_t dac_dc_value;               

/**** Functions **************************************************************/

void
remove_dc_voltage (void)
{
    calc_voltage();
    ema_gained_input_voltage();
    calc_dac_dc_voltage();
}   /* remove_dc_voltage() */

/*! \brief Calculate Vin1 (input voltage, OA0-) from Vout1 (PA22, OA0)
 *
 *  We use Vout1 and Vdac1 (DAC8) values to calculate equivalent input voltage.
 *  This is done for the fix gain of the first stage.
 *      1) Measure first gain stage output voltage Vout1, PA22.
 *      2) Calculate input voltage as: Vout1 = -G1*Vin1 + (1+G1)*Vdac1
 *                                     G1*Vin1 = -Vout1 + (1+G1)*Vdac1
 *  \return G1*Vin1
 */
static inline void
calc_voltage (void)
{
    /* ADC Channel ADC0.13 (OPA0 output) signal) */ 
    DL_ADC12_enableConversions(ADC12_0_INST);
    DL_ADC12_startConversion(ADC12_0_INST);

    /* ADC Channel ADC1.13 (OPA1 output) signal) */ 
    DL_ADC12_enableConversions(ADC12_1_INST);
    DL_ADC12_startConversion(ADC12_1_INST);

    DL_SYSCTL_setPowerPolicyRUN0SLEEP0();

    while(false == gCheckADC_0 && false == gCheckADC_1) {
          __WFE();
        }

    gOPA0_output = DL_ADC12_getMemResult(ADC12_0_INST, DL_ADC12_MEM_IDX_0);
    gOPA1_output = DL_ADC12_getMemResult(ADC12_1_INST, DL_ADC12_MEM_IDX_0); 

    gCheckADC_0 = false;
    gCheckADC_1 = false;

    /* Read output voltage: */ 
    /*
     * (G*Vin(n)/4096)*VDDA = -(Vout/4096)*VDDA +((G+1)*Vdac/256)*VDDA
     *
     * G*Vin(n) = -Vout + (G+1)*Vdac * 2^4
     *
     * Vout1 = ADC0.13
     * Vdac = DAC8.0
     * G = OPA0_Gain 
     */
 
    /* G*Vin(n) = (G + 1)*Vdac * 2^4 */                  
    gained_input_voltage_value = gDAC8_0_DATA * 16; 
    gained_input_voltage_value = gained_input_voltage_value * (OPA0_Gain+1);
    /* G*Vin(n) = G*Vin(n) - Vout */ 
    gained_input_voltage_value -= gOPA0_output;

}   /* calc_voltage() */

/*! \brief Calculates EMA on G*Vin(n).
 *
 *  Exponential moving average filter EMA is define as
 *  y(n) = alpha*x(n) + (1-alpha)*y(n-1)
 *
 *  \return EMA(G1*Vin1(n))
 */
static inline void
ema_gained_input_voltage (void)
{
    /* initial value of EMA filter */
   static bool ema_init = false;   
   /*
    *  EMA filter coefficient alpha = 1 / 2^n, for 1/256 => n = 8
    *  Reciprocal of alpha, 1 / alpha = 2^n
    *  Note: n depends on the cutoff frequency and sampling time.
    */
   /*
    * From previous conversion, replacing (n) with (n-1) accumulator has
    * following value: result = x(n-1) + (1 / alpha) * y(n-2) - y(n-2).
    * As this is recursive filter using output value from previous
    * conversion, that is same as: result = (1 / alpha) * y(n-1).
    *
    * x(n)             = gained_input_voltage_value
    * y(n-1) = y(n)    = ema_output_value_yn
    * [(1/alpha)*y(n)] = ema_alpha_output_value
    */
   static uint32_t ema_alpha_output_value = 0;

   if ( ema_init == false)   // First pass after reset, need to initialize EMA
   {
       /* y(0) = x(0) */ 
       ema_output_value_yn = gained_input_voltage_value;
       /* [(1/alpha)*y(0)] = (1/alpha)*y(0) */ 
       ema_alpha_output_value = ema_output_value_yn << SMOOTHING_FACTOR;
       /* EMA is initialized */
       ema_init = true;     
   }
   else
   {
        /* [(1/alpha)*y(n)] = [(1/alpha)*y(n-1)] - y(n-1) */ 
        ema_alpha_output_value -= ema_output_value_yn;
        /* [(1/alpha)*y(n)] = [(1/alpha)*y(n)] + x(n) */ 
        ema_alpha_output_value += gained_input_voltage_value;
        /* y(n-1) <= y(n) = [1/alpha*y(n)] / (1/alpha) */ 
        ema_output_value_yn = ema_alpha_output_value >> SMOOTHING_FACTOR;
   }
}   /* ema_calc_pir_voltage() */

/*! \brief Calc DAC
 *
 * Vdac(n) = (G*Vin(n) + Vout(n)) / (1 + G)
 *
 * G*Vin(n) = ema_output_value_yn
 * Desired Vout = FSR/2  
 *
 *  \return none
 */
static inline void
calc_dac_dc_voltage (void)
{
    /* Full Scale Range is 4096, 12 bit ADC */
#   define HALF_FSR     2048    
    
    if ((gOPA0_output < OPA0_output_min)||(gOPA0_output > OPA0_output_max))
    {
        /* EMA(G1*Vin(n)) */
        dac_dc_value = ema_output_value_yn;  
        /* Vout(n) =  FSR/2, desired */   
        dac_dc_value += (uint32_t) HALF_FSR;    
        /* dac_dc_value / (G1 + 1) */
        dac_dc_value= dac_dc_value /(OPA0_Gain+1);   
        /* Shift to 8-bit for DAC */   
        dac_dc_value >>= 4;      

        /* Limits based on ADC's range */
        if (dac_dc_value < 3){
            dac_dc_value = 3;
        }
        else if (dac_dc_value > 253)
        {
            dac_dc_value =253;
        }
        gDAC8_0_DATA= dac_dc_value;

        /* Set output voltage */ 
        DL_COMP_setDACCode0(COMP_0_INST, gDAC8_0_DATA);
        DL_COMP_enable(COMP_0_INST);

        /* software delay for DAC signals to settle:  100ms */
        delay_cycles(3000000);    
    }
}   /* calc_dac_dc_voltage */
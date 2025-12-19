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
//******************************************************************************************
//
//  main.c
//
//  Out of box demo for the MSPM0G3218 LaunchPad
//
//  This demo presents the basic peripherals of MSPM0G3218 (pressing S2 cycles to the next demo)
//  - Blink_LED: A blinking LED that toggles every second based on a timer
//  - RGB Cycling: each of the three colors are cycled through, changing every second to the 
//    next color in the RGB pattern.
//  - Thermistor ADC Read: The ADC takes a sample of the value of the thermistor circuit. If 
//    the value falls below 1500, the RGB LED turns Blue, green from 1500-2500, and red for >2500
// *******************************************************************************************
#include "ti_msp_dl_config.h"

#define RedLED   	(0)
#define GreenLED 	(1)
#define BlueLED  	(2)
#define LED_BLINK 	(0)
#define RGB_CYCLE 	(1)
#define ADC_READ 	(2)

volatile bool gCheckADC;
volatile uint16_t gAdcResult;
volatile uint8_t mode, count;

void readADC(void)
{
    DL_GPIO_setPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
    DL_GPIO_clearPins(GPIO_TEMP_PORT, GPIO_TEMP_R_LED_PIN);
    DL_GPIO_clearPins(GPIO_TEMP_PORT, GPIO_TEMP_G_LED_PIN);
    DL_GPIO_clearPins(GPIO_TEMP_PORT, GPIO_TEMP_B_LED_PIN);
    
  	/* Start the ADC0 conversion on pin PA22 */
  	DL_ADC12_startConversion(ADC12_0_INST);
	
  	/* Wait until we have acquired teh ADC data from MEM_IDX_0*/
    while (false == gCheckADC) {
        __WFE();
    }

    gAdcResult = DL_ADC12_getMemResult(ADC12_0_INST, DL_ADC12_MEM_IDX_0);

    if (gAdcResult > 2500) {
        DL_GPIO_setPins(GPIO_TEMP_PORT, GPIO_TEMP_R_LED_PIN);
        DL_GPIO_clearPins(GPIO_TEMP_PORT, GPIO_TEMP_G_LED_PIN);
        DL_GPIO_clearPins(GPIO_TEMP_PORT, GPIO_TEMP_B_LED_PIN);
    } else if (gAdcResult <= 2500 && gAdcResult > 1500) {
        DL_GPIO_setPins(GPIO_TEMP_PORT, GPIO_TEMP_G_LED_PIN);
        DL_GPIO_clearPins(GPIO_TEMP_PORT, GPIO_TEMP_R_LED_PIN);
        DL_GPIO_clearPins(GPIO_TEMP_PORT, GPIO_TEMP_B_LED_PIN);
    } else {
        DL_GPIO_setPins(GPIO_TEMP_PORT, GPIO_TEMP_B_LED_PIN);
        DL_GPIO_clearPins(GPIO_TEMP_PORT, GPIO_TEMP_R_LED_PIN);
        DL_GPIO_clearPins(GPIO_TEMP_PORT, GPIO_TEMP_G_LED_PIN);
    }

    gCheckADC = false;
  	/* Enable the ADC for the next iteration*/
    DL_ADC12_enableConversions(ADC12_0_INST);
}

void rgbLED(void)
{	/* Put LED pins in known state before cycling */
    DL_GPIO_setPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
    DL_GPIO_clearPins(GPIO_TEMP_PORT, GPIO_TEMP_R_LED_PIN);
    DL_GPIO_clearPins(GPIO_TEMP_PORT, GPIO_TEMP_G_LED_PIN);
    DL_GPIO_clearPins(GPIO_TEMP_PORT, GPIO_TEMP_B_LED_PIN);

    switch (count) {
        case RedLED:
        	/* Set LED to Red*/  
            DL_GPIO_setPins(GPIO_TEMP_PORT, GPIO_TEMP_R_LED_PIN);
        	/* Increment counter for next iteration*/    
        	count++; 
            break;

        case GreenLED:
        	/* Set LED to Green*/  
            DL_GPIO_setPins(GPIO_TEMP_PORT, GPIO_TEMP_G_LED_PIN);
        	/* Increment counter for next iteration*/  
            count++;
            break;

        case BlueLED:
        	/* Set LED to Blue*/  
            DL_GPIO_setPins(GPIO_TEMP_PORT, GPIO_TEMP_B_LED_PIN);
            /* Reset count after reaching last color in cycle */
        	count = 0;
            break;
    }
}

int main(void)
{
  /* Initialize variables and start in LED_BLINK mode */
    mode      = LED_BLINK;
    count     = 0;
    gCheckADC = false;

    SYSCFG_DL_init();
	
  	/* Enable interrupt on the S2 button */
    NVIC_EnableIRQ(GPIO_SWITCHES_INT_IRQN);
  	/* Start timer for LED_BLINK and RGB_CYCLE modes */
    DL_TimerA_startCounter(TIMER_0_INST);

    while (1) {
        switch (mode) {
            case LED_BLINK:  // Blink Red LED once per second
                /* Disable/enable interrupt routines based on mode*/
            	NVIC_DisableIRQ(ADC12_0_INST_INT_IRQN);
                NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);
                break;

            case RGB_CYCLE:  // RGB Color switch
            	/* Disable/enable interrupt routines based on mode*/
                NVIC_DisableIRQ(ADC12_0_INST_INT_IRQN);
                NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);
                break;

            case ADC_READ:  // PA22 ADC read thermistor and change RGB based on ADC value
                /* Disable/enable interrupt routines based on mode*/
            	NVIC_DisableIRQ(TIMER_0_INST_INT_IRQN);
                NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);
                readADC();
                break;
            default:
                break;
        }
    }
}

void ADC12_0_INST_IRQHandler(void)
{
    switch (DL_ADC12_getPendingInterrupt(ADC12_0_INST)) {
        case DL_ADC12_IIDX_MEM0_RESULT_LOADED:
            gCheckADC = true;
            break;
        default:
            break;
    }
}

void TIMER_0_INST_IRQHandler(void)
{
    switch (DL_TimerA_getPendingInterrupt(TIMER_0_INST)) {
        case DL_TIMERA_IIDX_ZERO:
            if (mode == LED_BLINK) {
              	/* Turn of the RGB LEDs*/
                DL_GPIO_clearPins(GPIO_TEMP_PORT, GPIO_TEMP_R_LED_PIN);
                DL_GPIO_clearPins(GPIO_TEMP_PORT, GPIO_TEMP_G_LED_PIN);
                DL_GPIO_clearPins(GPIO_TEMP_PORT, GPIO_TEMP_B_LED_PIN);
              	/* Begin toggling the red LED for MODE_BLINK*/
                DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
            } else if (mode == RGB_CYCLE) {
                rgbLED();
            }
            break;

        default:
            break;
    }
}

void GROUP1_IRQHandler(void)
{
    switch (DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1)) {
        case GPIO_SWITCHES_INT_IIDX:
            mode++;
            if (mode == ADC_READ) {
                mode = LED_BLINK;
            }

            break;

        default:
            break;
    }
}

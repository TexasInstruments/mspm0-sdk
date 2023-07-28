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

#define SAMPLE_ADC (0x01)
#define SEND_RESULTS (0x02)
#define SAMPLE_SEND_RESULTS (0x03)

/* Data for SPI to receive */
volatile uint8_t gCommand = 0x0;
volatile bool gNextCommand = false;
/* ADC variables */
volatile bool gCheckADC;
uint16_t gADCResult;
uint16_t startADCProcess(void);

int main(void)
{
    SYSCFG_DL_init();

    NVIC_ClearPendingIRQ(SPI_0_INST_INT_IRQN);
    NVIC_EnableIRQ(SPI_0_INST_INT_IRQN);
    NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);

    /*
     * Process the SPI command received.
     * The commands available are sample adc, send adc result, sample and send adc result.
     */
    while (1) {

        while(gNextCommand == false)
            __WFI();
        gNextCommand = false;

        switch(gCommand){
            case SAMPLE_ADC:
                gADCResult = startADCProcess();
                break;
            case SEND_RESULTS:
                DL_SPI_transmitDataBlocking16(SPI_0_INST, gADCResult);
                break;
            case SAMPLE_SEND_RESULTS:
                gADCResult = startADCProcess();
                DL_SPI_transmitDataBlocking16(SPI_0_INST, gADCResult);
                break;
            default:
                break;
        }

    }
}

/* This function is used to enable and start the conversion, then output the result. */ 
uint16_t startADCProcess(){

    uint16_t adcResult = 0;

    DL_ADC12_enableConversions(ADC12_0_INST);
    delay_cycles(4);
    DL_ADC12_startConversion(ADC12_0_INST);

    while(gCheckADC == false)
    {
        __WFI();
    }

    adcResult = DL_ADC12_getMemResult(ADC12_0_INST, DL_ADC12_MEM_IDX_0);

    return adcResult;
}

void SPI_0_INST_IRQHandler(void)
{
    switch (DL_SPI_getPendingInterrupt(SPI_0_INST)) {
        case DL_SPI_IIDX_RX:
            gCommand = DL_SPI_receiveDataBlocking16(SPI_0_INST);
            /* Toggle the LED after data reception */
            DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
            gNextCommand = true;
            break;
        default:
            break;
    }
}

void ADC12_0_INST_IRQHandler(void) {
  switch (DL_ADC12_getPendingInterrupt(ADC12_0_INST)) {
  case DL_ADC12_IIDX_MEM0_RESULT_LOADED:
    gCheckADC = true;
    break;
  default:
    break;
  }
}

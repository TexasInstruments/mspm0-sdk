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

#define ADC_SAMPLE_SIZE (64)

/* When FIFO is enabled, two 12bit samples are compacted into a single 32bit word */
#define ADC_FIFO_SAMPLES (ADC_SAMPLE_SIZE / 2)

uint16_t gADC0Samples[ADC_SAMPLE_SIZE];
uint16_t gADC1Samples[ADC_SAMPLE_SIZE];

volatile bool gADC0Done;
volatile bool gADC1Done;

int main(void) {

  SYSCFG_DL_init();

  /* Configure DMA source, destination and size */
  DL_DMA_setSrcAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t)DL_ADC12_getFIFOAddress(ADC12_0_INST));
  DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t)&gADC0Samples[0]);
  DL_DMA_setTransferSize(DMA, DMA_CH0_CHAN_ID, ADC_FIFO_SAMPLES);
  DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);

  DL_DMA_setSrcAddr(DMA, DMA_CH1_CHAN_ID, (uint32_t)DL_ADC12_getFIFOAddress(ADC12_1_INST));
  DL_DMA_setDestAddr(DMA, DMA_CH1_CHAN_ID, (uint32_t)&gADC1Samples[0]);
  DL_DMA_setTransferSize(DMA, DMA_CH1_CHAN_ID, ADC_FIFO_SAMPLES);
  DL_DMA_enableChannel(DMA, DMA_CH1_CHAN_ID);

  /* Enable interrupts for both ADCs */
  NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);
  NVIC_EnableIRQ(ADC12_1_INST_INT_IRQN);

  gADC0Done = false;
  gADC1Done = false;

  /* Start timer */
  DL_TimerG_startCounter(TIMER_0_INST);

  while (1) {
    while (gADC0Done == false || gADC1Done == false) {
      __WFI();
    }

    /* Breakpoint to check the buffers */
    __BKPT(0);

    /* Reconfigure DMA */
    DL_DMA_setSrcAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t)DL_ADC12_getFIFOAddress(ADC12_0_INST));
    DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t)&gADC0Samples[0]);
    DL_DMA_setTransferSize(DMA, DMA_CH0_CHAN_ID, ADC_FIFO_SAMPLES);
    DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);

    DL_DMA_setSrcAddr(DMA, DMA_CH1_CHAN_ID, (uint32_t)DL_ADC12_getFIFOAddress(ADC12_1_INST));
    DL_DMA_setDestAddr(DMA, DMA_CH1_CHAN_ID, (uint32_t)&gADC1Samples[0]);
    DL_DMA_setTransferSize(DMA, DMA_CH1_CHAN_ID, ADC_FIFO_SAMPLES);
    DL_DMA_enableChannel(DMA, DMA_CH1_CHAN_ID);

    gADC0Done = false;
    gADC1Done = false;
  }
}


void ADC12_0_INST_IRQHandler(void) {
  switch (DL_ADC12_getPendingInterrupt(ADC12_0_INST)) {
  case DL_ADC12_IIDX_DMA_DONE:
    gADC0Done = true;
    DL_ADC12_enableConversions(ADC12_0_INST);
    break;
  default:
    break;
  }
}

void ADC12_1_INST_IRQHandler(void) {
  switch (DL_ADC12_getPendingInterrupt(ADC12_1_INST)) {
  case DL_ADC12_IIDX_DMA_DONE:
    gADC1Done = true;
    DL_ADC12_enableConversions(ADC12_1_INST);
    break;
  default:
    break;
  }
}
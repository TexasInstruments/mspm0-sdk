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

/* When FIFO is enabled 2 samples are compacted in a single word */
#define ADC_FIFO_SAMPLES (ADC_SAMPLE_SIZE / 2)

uint16_t gADCSamplesPing[ADC_SAMPLE_SIZE];
uint16_t gADCSamplesPong[ADC_SAMPLE_SIZE];

volatile bool gCheckADC;

int main(void) {

  uint16_t maxReading;
  bool ping = true;
  uint16_t *ADCSamplesCurrDataPtr;

  SYSCFG_DL_init();

  /* Configure DMA source, destination and size */
  DL_DMA_setSrcAddr(DMA, DMA_CH0_CHAN_ID,
                    (uint32_t)DL_ADC12_getFIFOAddress(ADC12_0_INST));

  DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t)&gADCSamplesPing[0]);
  DL_DMA_setTransferSize(DMA, DMA_CH0_CHAN_ID, ADC_FIFO_SAMPLES);
  DL_DMA_Full_Ch_setEarlyInterruptThreshold(
      DMA, DMA_CH0_CHAN_ID, DL_DMA_EARLY_INTERRUPT_THRESHOLD_HALF);
  DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);

  /* Setup interrupts on device */
  NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);

  gCheckADC = false;

  DL_ADC12_enableConversions(ADC12_0_INST);
  DL_ADC12_startConversion(ADC12_0_INST);

  while (1) {

    while (false == gCheckADC) {
      __WFE();
    }
    // Breakpoint here to check the buffers and watch the ping pong operation.
    // The data should be alternating between each buffer.
    __BKPT(0);

    if (ping) { // Switch to pong

      ADCSamplesCurrDataPtr = gADCSamplesPing;

      DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t)&gADCSamplesPong[0]);
      DL_DMA_setTransferSize(DMA, DMA_CH0_CHAN_ID, ADC_FIFO_SAMPLES);
      DL_DMA_Full_Ch_setEarlyInterruptThreshold(
          DMA, DMA_CH0_CHAN_ID, DL_DMA_EARLY_INTERRUPT_THRESHOLD_HALF);
      DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);

      gCheckADC = false;
      ping = false;

      DL_ADC12_enableDMA(ADC12_0_INST);
      DL_ADC12_enableConversions(ADC12_0_INST);
      DL_ADC12_startConversion(ADC12_0_INST);

    } else { // Switch to Ping

      ADCSamplesCurrDataPtr = gADCSamplesPong;

      DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t)&gADCSamplesPing[0]);
      DL_DMA_setTransferSize(DMA, DMA_CH0_CHAN_ID, ADC_FIFO_SAMPLES);
      DL_DMA_Full_Ch_setEarlyInterruptThreshold(
          DMA, DMA_CH0_CHAN_ID, DL_DMA_EARLY_INTERRUPT_THRESHOLD_HALF);
      DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);

      gCheckADC = false;
      ping = true;

      DL_ADC12_enableDMA(ADC12_0_INST);
      DL_ADC12_enableConversions(ADC12_0_INST);
      DL_ADC12_startConversion(ADC12_0_INST);
    }
  }
}

void ADC12_0_INST_IRQHandler(void) {
  switch (DL_ADC12_getPendingInterrupt(ADC12_0_INST)) {
  case DL_ADC12_IIDX_DMA_DONE:
    gCheckADC = true;
    break;
  default:
    break;
  }
}

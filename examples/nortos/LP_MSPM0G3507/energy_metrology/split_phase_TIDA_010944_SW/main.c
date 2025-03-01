/*
 * Copyright (c) 2024, Texas Instruments Incorporated
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

//*****************************************************************************
// the includes
//*****************************************************************************
#include "TIDA_010944.h"

int main(void)
{
    SYSCFG_DL_init();
    HAL_init();
    TIDA_init(&gTidaHandle, &gMetrologyWorkingData, &gADSHandle, &gAMCHandle, &gDLT645);
    debugInit();

    DL_SPI_enableDMATransmitEvent(GEN_SPI_0_INST);
    NVIC_EnableIRQ(GEN_SPI_0_INST_INT_IRQN);
    NVIC_EnableIRQ(DMA_INT_IRQn);
    DLT645_UARTRxdmaInit(&gDLT645);

    TIDA_resetADC(&gTidaHandle);
    while(!ADS131M02_init(&gADSHandle));
    while(!AMC131M03_init(&gAMCHandle));

    Metrology_init(&gMetrologyWorkingData);
    TIDA_startDataCollection(&gTidaHandle);
    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
    NVIC_EnableIRQ(GEN_GPIO_IN_INT_IRQN);

    while (1)
    {
        TIDA_calculateMetrologyParameters(&gTidaHandle, &gMetrologyWorkingData, &gADSHandle, &gAMCHandle, &gDLT645);
    }
}



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

#include <ti/dali/dali_target_comm.h>
#include "ti_msp_dl_config.h"

#define SELECTOR_BIT  (uint8_t)(0x01)
#define DATA_SIZE 2

volatile bool gReceive = false;

volatile dali_controlGearVar gControlVar1;

uint8_t receivedData[DATA_SIZE] = {0};

int main(void)
{
    SYSCFG_DL_init();
    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
    /*DALI initialization
    wait for query regarding address and send address to controller*/
    DALI_Target_init();
    while(true){
        /* Check if there are any failures, and if so, address immediately */
        DALI_Target_failure();
        /* Receive data from master */
        DALI_Target_receive(&receivedData[0]);
        /* Transmit a response if required */
        DALI_Target_transmit();
        /* Check if information needs to be updated/backed up to flash */
        DALI_Target_flashUpdate();

    }
}


void UART_0_INST_IRQHandler(){
    switch(DL_UART_getPendingInterrupt(UART_0_INST)){
        case DL_UART_IIDX_RX:
            gReceive = true;
            break;
        case DL_UART_IIDX_ADDRESS_MATCH:
            /* Utilize this interrupt if using a hardware implementation of address matching */
            break;
        default:
            break;
    }
}





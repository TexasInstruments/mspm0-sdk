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

#include "ti_msp_dl_config.h"

volatile uint8_t gData = 0;

int main(void)
{
    SYSCFG_DL_init();

    /*
     * Invoke BSL is switch is off
     *
     */

    NVIC_EnableIRQ(GPIO_SWITCHES_INT_IRQN);
#ifdef UART_INTERFACE
    NVIC_ClearPendingIRQ(UART_0_INST_INT_IRQN);
    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
#endif
#ifdef I2C_INTERFACE
    NVIC_EnableIRQ(I2C_INST_INT_IRQN);
#endif


    while (1) {
        delay_cycles(12000000);
        DL_GPIO_togglePins(GPIO_LED_PORT,GPIO_LED_PIN_0_PIN);
    }
}

void GPIOA_IRQHandler(void)
{
    DL_SYSCTL_setShutdownStorageByte(DL_SYSCTL_SHUTDOWN_STORAGE_BYTE_0, 0x1);

    /* Issue reset to Start application */
    DL_SYSCTL_resetDevice(SYSCTL_RESETLEVEL_LEVEL_BOOT);
}


#ifdef UART_INTERFACE

void UART_0_INST_IRQHandler(void)
{
    switch (DL_UART_Main_getPendingInterrupt(UART_0_INST)) {
        case DL_UART_MAIN_IIDX_RX:
            gData = DL_UART_Main_receiveData(UART_0_INST);
            if (gData == 0x22) {
                /* Indicate that the BSL software trigger needed*/
                DL_SYSCTL_setShutdownStorageByte(DL_SYSCTL_SHUTDOWN_STORAGE_BYTE_0, 0x1);

                /* Issue reset to Start application */
                DL_SYSCTL_resetDevice(SYSCTL_RESETLEVEL_LEVEL_BOOT);
            }

            break;
        default:
            break;
    }
}

#endif

#ifdef I2C_INTERFACE
void I2C_INST_IRQHandler(void)
{
    switch (DL_I2C_getPendingInterrupt(I2C_INST)) {
        case DL_I2C_IIDX_TARGET_START:
            break;
        case DL_I2C_IIDX_TARGET_RXFIFO_TRIGGER:
            /* Store received data in buffer */
            while (DL_I2C_isTargetRXFIFOEmpty(I2C_INST) != true) {
                gData = DL_I2C_receiveTargetData(I2C_INST);
            }
            if (gData == 0x22) {
                /* Indicate that the BSL software trigger needed*/
                DL_SYSCTL_setShutdownStorageByte(DL_SYSCTL_SHUTDOWN_STORAGE_BYTE_0, 0x1);

                /* Issue reset to Start application */
                DL_SYSCTL_resetDevice(SYSCTL_RESETLEVEL_LEVEL_BOOT);
            }
            break;
        case DL_I2C_IIDX_TARGET_TXFIFO_TRIGGER:
            break;
        case DL_I2C_IIDX_TARGET_STOP:
            break;
        case DL_I2C_IIDX_TARGET_RX_DONE:
            /* Not used for this example */
        case DL_I2C_IIDX_TARGET_RXFIFO_FULL:
            /* Not used for this example */
        case DL_I2C_IIDX_TARGET_GENERAL_CALL:
            /* Not used for this example */
        case DL_I2C_IIDX_TARGET_EVENT1_DMA_DONE:
            /* Not used for this example */
        case DL_I2C_IIDX_TARGET_EVENT2_DMA_DONE:
            /* Not used for this example */
        default:
            break;
    }
}
#endif



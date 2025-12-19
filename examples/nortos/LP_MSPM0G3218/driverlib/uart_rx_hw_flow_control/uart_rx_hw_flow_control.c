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

/* Number of bytes for UART packet size */
#define UART_PACKET_SIZE (26)

/* Index to pause reception of UART data at */
#define PAUSE_TX_INDEX (10)

/* Flag for status of reception of data using automatic HW flow control */
volatile bool gAutoHWFlowControlDone = false;

/* Flags for checking status of pausing TX during automatic HW flow control */
volatile bool gPauseTransmission, gDataProcessed = false;

/* Data counter for gRxPacket1 */
volatile uint8_t gRxPacket1Counter = 0;

/* Data packets received from UART */
volatile uint8_t gRxPacket1[UART_PACKET_SIZE];
volatile uint8_t gRxPacket2[UART_PACKET_SIZE];

int main(void)
{
    uint32_t i;

    SYSCFG_DL_init();

    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);

    /*
     * Automatic RTS flow control is enabled, so the RTS flow control logic is
     * linked to the RX FIFO threshold level.
     */
    while (gAutoHWFlowControlDone == false) {
        /*
         * Spin in a while loop waiting for the RX interrupt to trigger, until all
         * expected data is received.
         */
        __WFI();

        /*
         * After receiving some bytes, emulate a delay by de-asserting RTS to
         * pause transmission of data
         */
        if (gPauseTransmission && !gDataProcessed) {
            /* Spin for some cycles to pause transmission between packets */
            delay_cycles(100000);

            gDataProcessed     = true;
            gPauseTransmission = false;

            /*
             * Read remaining bytes out of RX FIFO. This will re-assert RTS
             * and continue transmission.
             */
            while (!DL_UART_Main_isRXFIFOEmpty(UART_0_INST)) {
                gRxPacket1[gRxPacket1Counter++] =
                    DL_UART_Main_receiveData(UART_0_INST);
            }
        }
    }

    /*
     * Disable interrupts, re-configure the UART to disable automatic RTS flow
     * control, and then allow the user to manually set the RTS signal
     */
    DL_UART_Main_disableInterrupt(UART_0_INST,
        (DL_UART_MAIN_INTERRUPT_RX | DL_UART_MAIN_IIDX_RX_TIMEOUT_ERROR));
    NVIC_DisableIRQ(UART_0_INST_INT_IRQN);
    DL_UART_Main_changeConfig(UART_0_INST);
    DL_UART_Main_enable(UART_0_INST);
    DL_UART_Main_setFlowControl(UART_0_INST, DL_UART_MAIN_FLOW_CONTROL_NONE);

    /* Manually assert the RTS signal */
    DL_UART_Main_setRTSOutput(UART_0_INST, DL_UART_RTS_ASSERT);
    for (i = 0; i < UART_PACKET_SIZE; i++) {
        /*
         * After receiving some bytes, emulate a delay by de-asserting RTS to
         * pause transmission of data
         */
        if (i == PAUSE_TX_INDEX) {
            DL_UART_Main_setRTSOutput(UART_0_INST, DL_UART_MAIN_RTS_DEASSERT);

            /* Spin for some cycles to pause transmission */
            delay_cycles(100000);

            /* Re-assert RTS to signal to continue transmission */
            DL_UART_Main_setRTSOutput(UART_0_INST, DL_UART_MAIN_RTS_ASSERT);
        }

        gRxPacket2[i] = DL_UART_Main_receiveDataBlocking(UART_0_INST);

    }

    /*
     * All data has been received. Set a SW breakpoint to allow the user to
     * inspect the received data in gRxPacket1 and gRxPacket2.
     */
    __BKPT(0);

    /* Clear LED and USER_TEST pin */
    DL_GPIO_clearPins(
        GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN | GPIO_LEDS_USER_TEST_PIN);

    while (1) {
        __WFI();
    }
}

void UART_0_INST_IRQHandler(void)
{
    switch (DL_UART_Main_getPendingInterrupt(UART_0_INST)) {
        case DL_UART_MAIN_IIDX_RX_TIMEOUT_ERROR:
            /* Timeout has occurred, read any remaining bytes in RX FIFO */
        case DL_UART_MAIN_IIDX_RX:

            /*
             * Check if we should pause transmission. We do not read the RX
             * FIFO here so RTS stays de-asserted and transmission is paused.
             */
            if (gRxPacket1Counter >= PAUSE_TX_INDEX && !gDataProcessed) {
                gPauseTransmission = true;
                break;
            }

            /*
             * RTS is asserted (low) until the RX FIFO is filled up to the
             * threshold level. When the threshold level is reached and the
             * interrupt is triggered, then RTS is automatically de-asserted
             * (high), indicating there is no more room for data. After data
             * has been read out of the RX FIFO so it is filled less than the
             * threshold, then the RTS is re-asserted (low).
             */
            while (!DL_UART_Main_isRXFIFOEmpty(UART_0_INST)) {
                gRxPacket1[gRxPacket1Counter++] =
                    DL_UART_Main_receiveData(UART_0_INST);
                if (gRxPacket1Counter == UART_PACKET_SIZE) {
                    /* All expected data has been received */
                    gAutoHWFlowControlDone = true;
                }
            }
            break;
        default:
            break;
    }
}

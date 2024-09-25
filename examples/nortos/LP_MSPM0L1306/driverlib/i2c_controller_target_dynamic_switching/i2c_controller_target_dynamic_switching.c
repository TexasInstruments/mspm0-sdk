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

/* Maximum size of TX packet */
#define I2C_TX_MAX_PACKET_SIZE (16)

/* Number of bytes to send to Target device */
#define I2C_TX_PACKET_SIZE (16)

/* Maximum size of RX packet */
#define I2C_RX_MAX_PACKET_SIZE (16)

/* Number of bytes to receive from Target */
#define I2C_RX_PACKET_SIZE (16)

/* I2C Target address */
#define I2C_TARGET_ADDRESS (0x48)

/* Data sent to the Target */
uint8_t gTxPacket[I2C_TX_MAX_PACKET_SIZE] = {0x00, 0x01, 0x02, 0x03, 0x04,
    0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
/* Counters for TX length and bytes sent */
uint32_t gTxLen, gTxCount;

/* Data received from Target */
uint8_t gRxPacket[I2C_RX_MAX_PACKET_SIZE];
/* Counters for TX length and bytes sent */
uint32_t gRxLen, gRxCount;

/* Indicates status of I2C Controller */
enum I2cControllerStatus {
    I2C_STATUS_IDLE = 0,
    I2C_STATUS_TX_STARTED,
    I2C_STATUS_TX_INPROGRESS,
    I2C_STATUS_TX_COMPLETE,
    I2C_STATUS_RX_STARTED,
    I2C_STATUS_RX_INPROGRESS,
    I2C_STATUS_RX_COMPLETE,
    I2C_STATUS_ERROR,
} gI2cControllerStatus;

/* Define for I2C mode. Set to 0 to initialize the device in Target mode */
#define I2C_CONTROLLER_MODE (1)

/* Flags to track I2C mode */
volatile bool gControllerMode, gChangeMode;

/* Helper function where I2C is in Controller mode and writes to the Target */
void executeControllerMode(void);

/*
 * Helper function where I2C is in Target mode and receives from the
 * controller
 */
void executeTargetMode(void);

volatile bool gDetectStopCondition = false;

int main(void)
{
    gChangeMode = false;

    SYSCFG_DL_init();

    if (I2C_CONTROLLER_MODE) {
        /* Initialize I2C in controller mode */
        gControllerMode = true;

        /*
         * Set Blue LED, clear Green LED for controller mode by default.
         * Set LED to indicate start of transfer
         */
        DL_GPIO_setPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
        DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_2_PIN);

        DL_I2C_enableController(I2C_INST);
    } else {
        /* Initialize I2C in target mode */
        gControllerMode = false;

        /* Clear Blue LED, set Green LED for target mode by default */
        DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
        DL_GPIO_setPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_2_PIN);

        DL_I2C_enableTarget(I2C_INST);
    }

    NVIC_ClearPendingIRQ(I2C_INST_INT_IRQN);
    NVIC_EnableIRQ(I2C_INST_INT_IRQN);

    NVIC_EnableIRQ(GPIO_SWITCHES_INT_IRQN);

    DL_SYSCTL_disableSleepOnExit();

    while (1) {
        /* If the corresponding switch was pressed, change the I2C mode */
        if (gChangeMode == true) {
            if (gControllerMode == true) {
                /*
                 * If I2C is currently operating as the controller and the
                 * switch is pressed, change to target mode
                 */
                gControllerMode = false;

                /*
                 * Before further target initialization, check that the
                 * controller has finished transmission
                 */
                while (DL_I2C_getControllerStatus(I2C_INST) &
                       DL_I2C_CONTROLLER_STATUS_BUSY_BUS) {
                    ;
                }

                /* Reset and disable all I2C controller functionality */
                DL_I2C_resetControllerTransfer(I2C_INST);
                DL_I2C_disableController(I2C_INST);

                /*
                 * Disable the NVIC IRQ before switching to target mode.
                 * executeTargetMode sets the NVIC interrupt after enabling
                 * the TXFIFO interrupt
                 */
                NVIC_ClearPendingIRQ(I2C_INST_INT_IRQN);
                NVIC_DisableIRQ(I2C_INST_INT_IRQN);

                /* Enable target mode */
                DL_I2C_enableTarget(I2C_INST);

                /*
                 * Clear Blue LED, set Green LED to indicate I2C is operating
                 * in target mode
                 */
                DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
                DL_GPIO_setPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_2_PIN);
            } else {
                /*
                 * If I2C is currently operating as the target and the
                 * switch is pressed, change to controller mode
                 */
                gControllerMode = true;

                /*
                 * Before further controller initialization, check that the
                 * transmission is complete (receive STOP condition)
                 */
                while (gDetectStopCondition == false) {
                    ;
                }

                /* Disable target mode */
                DL_I2C_disableTarget(I2C_INST);

                /* Enable controller mode */
                DL_I2C_enableController(I2C_INST);

                /*
                 * Set Blue LED, clear Green LED to indicate I2C is operating
                 * in controller mode
                 */
                DL_GPIO_setPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
                DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_2_PIN);
            }

            gChangeMode = false;
        }

        gDetectStopCondition = false;

        /* These functions won't return unless a switch press is detected */
        if (gControllerMode == true) {
            executeControllerMode();
        } else {
            executeTargetMode();
        }
    }
}

/*
 * This function transmits to the target approximately every 1s.
 * After a successful transmission, the Blue LED will toggle.
 */
void executeControllerMode(void)
{
    /* Transmit until the user changes the mode by pressing the switch */
    while (gChangeMode == false) {
        gI2cControllerStatus = I2C_STATUS_IDLE;

        gTxLen = I2C_TX_PACKET_SIZE;
        /*
         * Fill the FIFO
         * The FIFO is 8-bytes deep, and this function will return number
         * of bytes written to FIFO
         */
        gTxCount =
            DL_I2C_fillControllerTXFIFO(I2C_INST, &gTxPacket[0], gTxLen);

        /* Enable TXFIFO trigger interrupt if there are more bytes to send */
        if (gTxCount < gTxLen) {
            DL_I2C_enableInterrupt(
                I2C_INST, DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER);
        } else {
            DL_I2C_disableInterrupt(
                I2C_INST, DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER);
        }

        /*
         * Send the packet to the target.
         * This function will send Start + Stop automatically.
         */
        gI2cControllerStatus = I2C_STATUS_TX_STARTED;
        while (!(DL_I2C_getControllerStatus(I2C_INST) &
                 DL_I2C_CONTROLLER_STATUS_IDLE)) {
            ;
        }
        DL_I2C_startControllerTransfer(I2C_INST, I2C_TARGET_ADDRESS,
            DL_I2C_CONTROLLER_DIRECTION_TX, gTxLen);

        /* Wait until the Controller sends all bytes */
        while ((gI2cControllerStatus != I2C_STATUS_TX_COMPLETE) &&
               (gI2cControllerStatus != I2C_STATUS_ERROR)) {
            __WFE();
        }

        while (DL_I2C_getControllerStatus(I2C_INST) &
               DL_I2C_CONTROLLER_STATUS_BUSY_BUS) {
            ;
        }

        /* Trap if there was an error */
        if (DL_I2C_getControllerStatus(I2C_INST) &
            DL_I2C_CONTROLLER_STATUS_ERROR) {
            /* Blue LED will remain high if there is an error */
            __BKPT(0);
        }

        /* If write was successful, toggle Blue LED */
        DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);

        /* Delay for 1s */
        delay_cycles(32000000);
    }
}

/*
 * This function responds to write requests from an I2C controller.
 * The Green LED will toggle after successful transactions.
 */
void executeTargetMode(void)
{
    /* Initialize variables to receive data inside RX ISR */
    gRxCount = 0;
    gRxLen   = I2C_RX_MAX_PACKET_SIZE;

    NVIC_EnableIRQ(I2C_INST_INT_IRQN);

    while (gChangeMode == false) {
        ;
    }
}

void I2C_INST_IRQHandler(void)
{
    static bool dataRx = false;

    switch (DL_I2C_getPendingInterrupt(I2C_INST)) {
        case DL_I2C_IIDX_CONTROLLER_RX_DONE:
            gI2cControllerStatus = I2C_STATUS_RX_COMPLETE;
            break;
        case DL_I2C_IIDX_CONTROLLER_TX_DONE:
            DL_I2C_disableInterrupt(
                I2C_INST, DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER);
            gI2cControllerStatus = I2C_STATUS_TX_COMPLETE;
            break;
        case DL_I2C_IIDX_CONTROLLER_RXFIFO_TRIGGER:
            /*
             * This example doesn't implement Controller reception, but the
             * code below can be used as a reference
             */
            gI2cControllerStatus = I2C_STATUS_RX_INPROGRESS;
            /* Receive all bytes from target */
            while (DL_I2C_isControllerRXFIFOEmpty(I2C_INST) != true) {
                if (gRxCount < gRxLen) {
                    gRxPacket[gRxCount++] =
                        DL_I2C_receiveControllerData(I2C_INST);
                } else {
                    /* Ignore and remove from FIFO if the buffer is full */
                    DL_I2C_receiveControllerData(I2C_INST);
                }
            }
            break;
        case DL_I2C_IIDX_CONTROLLER_TXFIFO_TRIGGER:
            gI2cControllerStatus = I2C_STATUS_TX_INPROGRESS;
            /* Fill TX FIFO with next bytes to send */
            if (gTxCount < gTxLen) {
                gTxCount += DL_I2C_fillControllerTXFIFO(
                    I2C_INST, &gTxPacket[gTxCount], gTxLen - gTxCount);
            }
            break;
        case DL_I2C_IIDX_CONTROLLER_NACK:
            if ((gI2cControllerStatus == I2C_STATUS_RX_STARTED) ||
                (gI2cControllerStatus == I2C_STATUS_TX_STARTED)) {
                /* NACK interrupt if I2C Target is disconnected */
                gI2cControllerStatus = I2C_STATUS_ERROR;
            }
            break;

        /* The Controller cases below aren't used in this example */
        case DL_I2C_IIDX_CONTROLLER_ARBITRATION_LOST:
        case DL_I2C_IIDX_CONTROLLER_RXFIFO_FULL:
        case DL_I2C_IIDX_CONTROLLER_TXFIFO_EMPTY:
        case DL_I2C_IIDX_CONTROLLER_START:
        case DL_I2C_IIDX_CONTROLLER_STOP:
        case DL_I2C_IIDX_CONTROLLER_EVENT1_DMA_DONE:
        case DL_I2C_IIDX_CONTROLLER_EVENT2_DMA_DONE:
            break;

        case DL_I2C_IIDX_TARGET_START:
            /* Upon START condition, clear state-tracking variables */
            gTxCount = 0;
            gRxCount = 0;
            dataRx   = false;

            /* Flush TX FIFO to refill it */
            DL_I2C_flushTargetTXFIFO(I2C_INST);
            break;
        case DL_I2C_IIDX_TARGET_RXFIFO_TRIGGER:
            /* Store received data in buffer */
            dataRx = true;

            while (DL_I2C_isTargetRXFIFOEmpty(I2C_INST) != true) {
                if (gRxCount < gRxLen) {
                    gRxPacket[gRxCount++] = DL_I2C_receiveTargetData(I2C_INST);
                } else {
                    /* Prevent overflow and just ignore data */
                    DL_I2C_receiveTargetData(I2C_INST);
                }
            }
            break;
        case DL_I2C_IIDX_TARGET_TXFIFO_TRIGGER:
            /*
             * This example doesn't implement Target transmission, but the
             * code below can be used as a reference
             */

            /* Fill TX FIFO if there are more bytes to send */
            if (gTxCount < gTxLen) {
                gTxCount += DL_I2C_fillTargetTXFIFO(
                    I2C_INST, &gTxPacket[gTxCount], (gTxLen - gTxCount));
            } else {
                /*
                 * Fill FIFO with 0x00 if more data is requested than
                 * expected gTxLen
                 */
                while (
                    DL_I2C_transmitTargetDataCheck(I2C_INST, 0x00) != false) {
                    ;
                }
            }
            break;
        case DL_I2C_IIDX_TARGET_STOP:
            /* If data was received, echo to TX buffer */
            if (dataRx == true) {
                for (uint16_t i = 0;
                     (i < gRxCount) && (i < I2C_TX_MAX_PACKET_SIZE); i++) {
                    gTxPacket[i] = gRxPacket[i];
                    DL_I2C_flushTargetTXFIFO(I2C_INST);
                }

                dataRx = false;
            }

            /* Toggle Green LED to indicate successful RX or TX */
            DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_2_PIN);

            gDetectStopCondition = true;

            break;

        /* The Target cases below aren't used in this example */
        case DL_I2C_IIDX_TARGET_RX_DONE:
        case DL_I2C_IIDX_TARGET_TX_DONE:
        case DL_I2C_IIDX_TARGET_RXFIFO_FULL:
        case DL_I2C_IIDX_TARGET_TXFIFO_EMPTY:
        case DL_I2C_IIDX_TARGET_GENERAL_CALL:
        case DL_I2C_IIDX_TARGET_EVENT1_DMA_DONE:
        case DL_I2C_IIDX_TARGET_EVENT2_DMA_DONE:
        default:
            break;
    }
}

void GROUP1_IRQHandler(void)
{
    switch (DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1)) {
        case GPIO_SWITCHES_INT_IIDX:
            switch (DL_GPIO_getPendingInterrupt(GPIO_SWITCHES_PORT)) {
                case GPIO_SWITCHES_USER_SWITCH_1_IIDX:
                    /*
                     * When USER_SWITCH_1_INST is pressed, set flag to change
                     * the I2C mode
                     */
                    gChangeMode = true;
                    break;
                default:
                    break;
            }

            break;
        default:
            break;
    }
}

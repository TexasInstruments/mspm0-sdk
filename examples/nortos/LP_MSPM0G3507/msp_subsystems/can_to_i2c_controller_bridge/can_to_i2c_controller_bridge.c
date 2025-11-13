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
#include "user_define.h"

/* Variables for ItoC_FIFO
 * ItoC_FIFO is used to temporarily store message from I2C to CAN */
Custom_Element gItoC_FIFO[ItoC_FIFO_SIZE];
Custom_FIFO gI2c2Can_FIFO = {0, 0, 0, gItoC_FIFO};

/* Variables for C2I_FIFO
 * C2I_FIFO is used to temporarily store message from CAN to I2C */
Custom_Element gC2I_FIFO[C2I_FIFO_SIZE];
Custom_FIFO gCan2I2c_FIFO = {0, 0, 0, gC2I_FIFO};

int main(void)
{
    SYSCFG_DL_init();

    gInterruptLine1Status = 0;

    /* Enable Interrupts */
    DL_I2C_enableInterrupt(I2C_INST, DL_I2C_INTERRUPT_TARGET_TXFIFO_TRIGGER);
    NVIC_ClearPendingIRQ(CANFD0_INT_IRQn);
    NVIC_ClearPendingIRQ(I2C_INST_INT_IRQN);
    NVIC_EnableIRQ(CANFD0_INT_IRQn);
    NVIC_EnableIRQ(I2C_INST_INT_IRQN);
    NVIC_EnableIRQ(GPIO_SWITCHES_INT_IRQN);

    while (DL_MCAN_OPERATION_MODE_NORMAL != DL_MCAN_getOpMode(CANFD0))
        ;

    while (1) {
        /* Send received CAN message to I2C */
        if(gCan2I2c_FIFO.fifo_count>0 && gI2CTxflag == false)
        {
            /* Get gI2C_TX_Element from C2I_FIFO
             * Update indicator variable of C2I_FIFO */
            gI2C_TX_Element = gCan2I2c_FIFO.fifo_pointer[gCan2I2c_FIFO.fifo_out];
            gCan2I2c_FIFO.fifo_count--;
            gCan2I2c_FIFO.fifo_out++;
            if(gCan2I2c_FIFO.fifo_out >= C2I_FIFO_SIZE)gCan2I2c_FIFO.fifo_out = 0;
#ifdef I2C_TRANSPARENT     /* I2C Transparent transmission */
            /* Convert message format */
            uint8_t i2cTargetAddress;
            i2cTargetAddress = processI2CTxMsg_controller_transparent(gI2CTransmitGroup, &gI2C_TX_Element);
            /* Send messages via I2C */
            gI2CTxflag = true;
            gTxLen = (gI2C_TX_Element.dlc);
            sendI2CTxMsg_controller(gI2CTransmitGroup, i2cTargetAddress);
#else       /* I2C Protocol transmission */
            /* Convert message format */
            uint8_t i2cTargetAddress;
            i2cTargetAddress = processI2CTxMsg_controller(gI2CTransmitGroup, &gI2C_TX_Element);
            /* Send messages via I2C */
            gI2CTxflag = true;
            gTxLen = (3 + I2C_ID_LENGTH + gI2C_TX_Element.dlc);
            sendI2CTxMsg_controller(gI2CTransmitGroup, i2cTargetAddress);
#endif
        }

        /* Send received I2C message to CAN */
        if(gI2c2Can_FIFO.fifo_count>0)
        {
            /* Get gCAN_TX_Element from ItoC_FIFO
             * Update indicator variable of I2C2C_FIFO */
            gCAN_TX_Element = gI2c2Can_FIFO.fifo_pointer[gI2c2Can_FIFO.fifo_out];
            gI2c2Can_FIFO.fifo_count--;
            gI2c2Can_FIFO.fifo_out++;
            if(gI2c2Can_FIFO.fifo_out >= ItoC_FIFO_SIZE)
                gI2c2Can_FIFO.fifo_out = 0;
            /* Convert message format */
            processCANTxMsg(&txMsg0, &gCAN_TX_Element);
            /* Send messages via CAN */
            sendCANTxMsg(&txMsg0);
        }
    }
}

void GROUP1_IRQHandler(void)
{
    switch (DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1)) {
        case GPIO_SWITCHES_INT_IIDX:
            /* If SW is low, send the read command */
            if (!DL_GPIO_readPins(GPIO_SWITCHES_PORT, GPIO_SWITCHES_USER_SWITCH_1_PIN))
            {
#ifdef I2C_TRANSPARENT     /* I2C Transparent transmission */
                /* Start I2C read, Use default I2C targer address*/
                readI2CRxMsg_controller(I2C_TRANSPARENT_LENGTH, I2C_TARGET_ADDRESS);
                DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
#else       /* I2C Protocol transmission */
                /* Start I2C read, Use default I2C targer address*/
                readI2CRxMsg_controller(I2C_RX_SIZE, I2C_TARGET_ADDRESS);
                DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
#endif
            }
            break;
    }
}

void CANFD0_IRQHandler(void)
{
    bool processFlag;
    switch (DL_MCAN_getPendingInterrupt(CANFD0)) {
        case DL_MCAN_IIDX_LINE1:
            /* Check MCAN interrupts fired during TX/RX of CAN package */
            gInterruptLine1Status |= DL_MCAN_getIntrStatus(CANFD0);
            DL_MCAN_clearIntrStatus(
                CANFD0, gInterruptLine1Status, DL_MCAN_INTR_SRC_MCAN_LINE_1);
            processFlag = getCANRxMsg(&rxMsg);
            if( gCan2I2c_FIFO.fifo_count >= C2I_FIFO_SIZE)
            {
                /* When the CAN bus is overloaded, stop receiving messages */
                processFlag = false;
                /* Overload handle */
                //__BKPT(0);
            }
            if(processFlag == true)
            {
                /*Convert the received message to gCAN_RX_Element */
                processCANRxMsg(&rxMsg, &gCAN_RX_Element);
                /* Store gCAN_RX_Element into C2S_FIFO
                 * Update indicator variable of C2S_FIFO */
                gCan2I2c_FIFO.fifo_pointer[gCan2I2c_FIFO.fifo_in] = gCAN_RX_Element;
                gCan2I2c_FIFO.fifo_count++;
                gCan2I2c_FIFO.fifo_in++;
                if(gCan2I2c_FIFO.fifo_in >= C2I_FIFO_SIZE)gCan2I2c_FIFO.fifo_in = 0;
            }
            break;
        default:
            break;
    }
}
void I2C_INST_IRQHandler(void)
{
    bool processFlag;
    switch (DL_I2C_getPendingInterrupt(I2C_INST)) {
        case DL_I2C_IIDX_CONTROLLER_RXFIFO_TRIGGER:
#ifdef I2C_TRANSPARENT     /* I2C Transparent transmission */
            /* Store received data in buffer */
            while (DL_I2C_isControllerRXFIFOEmpty(I2C_INST) != true)
            {
                getI2CRxMsg_controller_transparent(gI2CReceiveGroup);
            }
#else       /* I2C Protocol transmission */
            /* Store received data in buffer */
            while (DL_I2C_isControllerRXFIFOEmpty(I2C_INST) != true)
            {
                processFlag = getI2CRxMsg_controller(gI2CReceiveGroup);
                if(gI2c2Can_FIFO.fifo_count >= ItoC_FIFO_SIZE)
                {
                    /* When the I2C is overloaded, stop receiving messages */
                    processFlag = false;
                    /* Overload handle*/
                    //__BKPT(0);
                }
                if(processFlag == true)
                {
                    /*Convert the received message to gI2C_RX_Element */
                    processI2CRxMsg_controller(gI2CReceiveGroup, &gI2C_RX_Element, I2C_TARGET_ADDRESS);
                    /* Store gI2C_RX_Element into ItoC_FIFO
                     * Update indicator variable of ItoC_FIFO */
                    gI2c2Can_FIFO.fifo_pointer[gI2c2Can_FIFO.fifo_in] = gI2C_RX_Element;
                    gI2c2Can_FIFO.fifo_count++;
                    gI2c2Can_FIFO.fifo_in++;
                    if(gI2c2Can_FIFO.fifo_in >= ItoC_FIFO_SIZE)
                        gI2c2Can_FIFO.fifo_in = 0;
                    processFlag = false;
                }
            }
#endif
            break;
        case DL_I2C_IIDX_CONTROLLER_TXFIFO_TRIGGER:
            /* Fill TX FIFO with next bytes to send */
            if (gTxCount < gTxLen) {
                gTxCount += DL_I2C_fillControllerTXFIFO(
                    I2C_INST, &gI2CTransmitGroup[gTxCount], gTxLen - gTxCount);
            }
            break;
        case DL_I2C_IIDX_CONTROLLER_NACK:
            break;
        case DL_I2C_IIDX_CONTROLLER_TX_DONE:
            break;
        case DL_I2C_IIDX_CONTROLLER_RX_DONE:
            break;
        case DL_I2C_IIDX_CONTROLLER_STOP:
            if(gI2CTxflag == true)
            {
                /* For Tx done, clear gI2CTxflag*/
                DL_I2C_disableInterrupt(
                    I2C_INST, DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER);
                gI2CTxflag = false;
            }
            else
            {
#ifdef I2C_TRANSPARENT     /* I2C Transparent transmission */
                /* For Transparent Rx done, receive the message */
                processFlag = true;
                if(gI2c2Can_FIFO.fifo_count >= ItoC_FIFO_SIZE)
                {
                    /* When the I2C is overloaded, stop receiving messages */
                    processFlag = false;
                    /* Overload handle*/
                    //__BKPT(0);
                }
                if(processFlag == true)
                {
                    /*Convert the received message to gI2C_RX_Element */
                    processI2CRxMsg_controller_transparent(gI2CReceiveGroup, &gI2C_RX_Element, I2C_TARGET_ADDRESS);
                    /* Store gI2C_RX_Element into ItoC_FIFO
                     * Update indicator variable of ItoC_FIFO */
                    gI2c2Can_FIFO.fifo_pointer[gI2c2Can_FIFO.fifo_in] = gI2C_RX_Element;
                    gI2c2Can_FIFO.fifo_count++;
                    gI2c2Can_FIFO.fifo_in++;
                    if(gI2c2Can_FIFO.fifo_in >= ItoC_FIFO_SIZE)
                        gI2c2Can_FIFO.fifo_in = 0;
                    processFlag = false;
                    gGetI2cRxMsg_Count = 0;
                }
#endif
            }
            break;
        default:
            break;
    }
}

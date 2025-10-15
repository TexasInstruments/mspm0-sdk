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

#include <expander.h>
#include <stdint.h>
#include "ti_msp_dl_config.h"


extern volatile tDataType ioExpander;
/* Initialize the current and last GPIO states to be equal */
uint8_t gGpioState = 0xFF;
uint8_t gLastGpioState = 0xFF;
/* Initialize varbiable that stores GPIO direction to all inputs */
uint8_t gGpioDirection = 0xFF;
uint8_t gGpioOutput;

//System Init
void systemInit(void)
{
    //GPIO init
    DL_GPIO_init(GPIO_GRP_0_PIN_0_IOMUX);
    DL_GPIO_init(GPIO_GRP_0_PIN_1_IOMUX);
    DL_GPIO_init(GPIO_GRP_0_PIN_2_IOMUX);
    DL_GPIO_init(GPIO_GRP_0_PIN_3_IOMUX);
    DL_GPIO_init(GPIO_GRP_0_PIN_4_IOMUX);
    DL_GPIO_init(GPIO_GRP_0_PIN_5_IOMUX);
    DL_GPIO_init(GPIO_GRP_0_PIN_6_IOMUX);
    DL_GPIO_init(GPIO_GRP_0_PIN_7_IOMUX);

    //Interrupt init
    NVIC_EnableIRQ(I2C_INST_INT_IRQN);
}

// GPIO Init
void DL_GPIO_init(uint32_t pincmIndex)
{
    IOMUX->SECCFG.PINCM[pincmIndex] =
        (IOMUX_PINCM_INENA_ENABLE | IOMUX_PINCM_PC_CONNECTED | DL_GPIO_RESISTOR_PULL_UP | ((uint32_t) 0x00000001));
}

// GPIO direction
void gpioDirectionSet(void)
{
    gGpioDirection = ioExpander.rxDataPkt[1];

    if(gGpioDirection & 0x1)
    {
        DL_GPIO_enableOutput(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_0_PIN);
    }
    else
    {
        DL_GPIO_disableOutput(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_0_PIN);
    }

    if((gGpioDirection >> 1) & 0x1)
    {
        DL_GPIO_enableOutput(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_1_PIN);
    }
    else
    {
        DL_GPIO_disableOutput(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_1_PIN);
    }

    if((gGpioDirection >> 2) & 0x1)
    {
        DL_GPIO_enableOutput(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_2_PIN);
    }
    else
    {
        DL_GPIO_disableOutput(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_2_PIN);
    }

    if((gGpioDirection >> 3) & 0x1)
    {
        DL_GPIO_enableOutput(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_3_PIN);
    }
    else
    {
        DL_GPIO_disableOutput(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_3_PIN);
    }

    if((gGpioDirection >> 4) & 0x1)
    {
        DL_GPIO_enableOutput(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_4_PIN);
    }
    else
    {
        DL_GPIO_disableOutput(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_4_PIN);
    }

    if((gGpioDirection >> 5) & 0x1)
    {
        DL_GPIO_enableOutput(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_5_PIN);
    }
    else
    {
        DL_GPIO_disableOutput(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_5_PIN);
    }

    if((gGpioDirection >> 6) & 0x1)
    {
        DL_GPIO_enableOutput(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_6_PIN);
    }
    else
    {
        DL_GPIO_disableOutput(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_6_PIN);
    }

    if((gGpioDirection >> 7) & 0x1)
    {
        DL_GPIO_enableOutput(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_7_PIN);
    }
    else
    {
        DL_GPIO_disableOutput(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_7_PIN);
    }
    NVIC_EnableIRQ(GPIOA_INT_IRQn);
}

// GPIO output level
void gpioOutputCtl(void)
{
    gGpioOutput = ioExpander.rxDataPkt[1];

    if(gGpioOutput & 0x1)
    {
        DL_GPIO_setPins(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_0_PIN);
    }
    else
    {
        DL_GPIO_clearPins(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_0_PIN);
    }

    if((gGpioOutput >> 1) & 0x1)
    {
        DL_GPIO_setPins(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_1_PIN);
    }
    else
    {
        DL_GPIO_clearPins(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_1_PIN);
    }

    if((gGpioOutput >> 2) & 0x1)
    {
        DL_GPIO_setPins(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_2_PIN);
    }
    else
    {
        DL_GPIO_clearPins(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_2_PIN);
    }

    if((gGpioOutput >> 3) & 0x1)
    {
        DL_GPIO_setPins(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_3_PIN);
    }
    else
    {
        DL_GPIO_clearPins(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_3_PIN);
    }

    if((gGpioOutput >> 4) & 0x1)
    {
        DL_GPIO_setPins(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_4_PIN);
    }
    else
    {
        DL_GPIO_clearPins(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_4_PIN);
    }

    if((gGpioOutput >> 5) & 0x1)
    {
        DL_GPIO_setPins(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_5_PIN);
    }
    else
    {
        DL_GPIO_clearPins(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_5_PIN);
    }

    if((gGpioOutput >> 6) & 0x1)
    {
        DL_GPIO_setPins(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_6_PIN);
    }
    else
    {
        DL_GPIO_clearPins(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_6_PIN);
    }

    if((gGpioOutput >> 7) & 0x1)
    {
        DL_GPIO_setPins(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_7_PIN);
    }
    else
    {
        DL_GPIO_clearPins(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_7_PIN);
    }
}

// GPIO Status read
uint8_t DL_GPIO_readPinStatus(uint32_t pins)
{
    if(DL_GPIO_readPins(GPIO_GRP_0_PORT,pins))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void receivePacket(uint8_t u8Data)
{
    switch (ioExpander.prtclType)
    {
        case eCommd:
            ioExpander.rxDataPkt[0] = u8Data;
            ioExpander.prtclType = eData;
            break;
        case eData:
            ioExpander.rxDataPkt[1] = u8Data;
            ioExpander.pktTyp = ioExpander.rxDataPkt[0];
            ioExpander.prtclType = eCommd;
            break;
        default:
            break;
     }
}

//I2C INT
void I2C_INST_IRQHandler(void)
{
    switch (DL_I2C_getPendingInterrupt(I2C_INST)) {
        case DL_I2C_IIDX_TARGET_START:
            /* Fill TX FIFO with current pin state */
            DL_I2C_fillTargetTXFIFO(I2C_INST, &gGpioState, 1);
            break;
        case DL_I2C_IIDX_TARGET_RXFIFO_TRIGGER:
            /* Store received data in buffer */
            while (DL_I2C_isTargetRXFIFOEmpty(I2C_INST) != true) {
                receivePacket(DL_I2C_receiveTargetData(I2C_INST));
            }
            break;
        case DL_I2C_IIDX_TARGET_TX_DONE:
            /* Pull interrupt pin high when Controller reads from IO Expander */
            DL_GPIO_setPins(GPIO_GRP_0_PORT, GPIO_GRP_0_IRQ_OUT_PIN);
            break;
        case DL_I2C_IIDX_TARGET_STOP:
            /* Flush TX FIFO */
            DL_I2C_flushTargetTXFIFO(I2C_INST);
            break;
        default:
            break;
    }
}

void GPIOA_IRQHandler(void)
{
    /* Store the current pin state */
    gGpioState = (DL_GPIO_readPinStatus(GPIO_GRP_0_PIN_7_PIN) << 7) |
                 (DL_GPIO_readPinStatus(GPIO_GRP_0_PIN_6_PIN) << 6) |
                 (DL_GPIO_readPinStatus(GPIO_GRP_0_PIN_5_PIN) << 5) |
                 (DL_GPIO_readPinStatus(GPIO_GRP_0_PIN_4_PIN) << 4) |
                 (DL_GPIO_readPinStatus(GPIO_GRP_0_PIN_3_PIN) << 3) |
                 (DL_GPIO_readPinStatus(GPIO_GRP_0_PIN_2_PIN) << 2) |
                 (DL_GPIO_readPinStatus(GPIO_GRP_0_PIN_1_PIN) << 1) |
                 (DL_GPIO_readPinStatus(GPIO_GRP_0_PIN_0_PIN));

    /* Loop through all pins */
    for (int i = 0; i < 8; i++) {
        /* Check if the current pin state changed */
        if (((gGpioState >> i) & 0x1) != ((gLastGpioState >> i) & 0x1)) {
            /* If the pin is an Input */
            if (((gGpioDirection >> i) & 0x1) == 0) {
                /* Pull interrupt pin LOW when an input pin state changes */
                DL_GPIO_clearPins(GPIO_GRP_0_PORT, GPIO_GRP_0_IRQ_OUT_PIN);
            }
        }
    }
    gLastGpioState = gGpioState;
}

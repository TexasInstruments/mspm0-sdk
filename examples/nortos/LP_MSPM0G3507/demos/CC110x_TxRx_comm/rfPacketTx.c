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
#include "spi_cc11xL.h"
#include "smartrf_settings.h"
#include <stdlib.h>

#define PKTLEN                  15  // The packet length should not be set higher than 61 if we want the complete packet to fit in the
                                    // FIFO on the RX side:
                                    // 1 length byte + 61 payload bytes + 2 status bytes = 64 bytes

static void initMCU(void);
static void registerConfig(void);
static void runTX(void);
static void createPacket(uint8_t txBuffer[]);

static bool packetSent = false;
static bool buttonPushed = false;
static uint32_t packetCounter = 0;


//-----------------------------------------------------------------------------
// main
//-----------------------------------------------------------------------------
int main(void)
{
    // Initialize MCU and peripherals
    initMCU();

    // Write radio registers
    registerConfig();

    // Enter runTX, never coming back
    runTX();
}


//-----------------------------------------------------------------------------
// createPacket
//
// This function is called before a packet is transmitted. It fills the
// txBuffer with a packet consisting of a length byte, two bytes packet counter
// and n random bytes.
//
// The packet format is as follows:
// |--------------------------------------------------------------|
// |           |           |           |         |       |        |
// | pktLength | pktCount1 | pktCount0 | rndData |.......| rndData|
// |           |           |           |         |       |        |
// |--------------------------------------------------------------|
// txBuffer[0] txBuffer[1] txBuffer[2]            txBuffer[PKTLEN]
//
// Input Parameters
//
//  - *txBuffer     - Pointer to start of txBuffer
//
// Return
//
//  - None
//-----------------------------------------------------------------------------
static void createPacket(uint8_t txBuffer[])
{
    txBuffer[0] = PKTLEN;                           // Length byte
    txBuffer[1] = (uint8_t)(packetCounter >> 8);    // MSB of packetCounter
    txBuffer[2] = (uint8_t)packetCounter;           // LSB of packetCounter

    // Fill rest of buffer with random bytes
    for(uint8_t i = 3; i < (PKTLEN + 1); i++)
    {
        txBuffer[i] = (uint8_t)rand();
    }
}


//-----------------------------------------------------------------------------
// runTX
//
// Continuously sends packets on button push until button is pushed again.
// After the radio has gone into TX the function waits for interrupt that
// packet has been sent. Updates packet counter and toggle LED for for
// each packet sent.
//
// Input Parameters
//
//  - None
//
// Return
//
//  - None
//-----------------------------------------------------------------------------
static void runTX(void)
{
    // Initialize packet buffer of size PKTLEN + 1
    uint8_t txBuffer[PKTLEN + 1] = {0};

    DL_GPIO_clearInterruptStatus(GPIOB, GPIO_INPUT_GDO0_PIN |
                                        GPIO_BUTTON_S2_PIN);

    // Enable Interrupt for GPIOB port
    NVIC_EnableIRQ(GPIO_MULTIPLE_GPIOB_INT_IRQN);

    // Infinite loop
    while(true)
    {
        // Wait for button push
        while (!buttonPushed)
        {
            __WFI();
        }
        buttonPushed = false;

        // Continuously sent packets until button is pressed
        do {
                // update packet counter
                packetCounter++;

                // Create a random packet with PKTLEN + 2 byte packet
                // counter + n x random bytes
                createPacket(txBuffer);

                // Write packet to TX FIFO
                cc11xLSpiWriteTxFifo(txBuffer,sizeof(txBuffer));

                // Strobe TX to send packet
                mspm0SpiCmdStrobe(CC11xL_STX);

                // Wait for interrupt that packet has been sent.
                while (!packetSent)
                {
                    __WFI();
                }
                packetSent = false;

                // Toggle LED
                DL_GPIO_togglePins(GPIO_LED_PORT, GPIO_LED_GLED_PIN);

                delay_cycles(16000000/5); // 0.1 s
        } while (!buttonPushed);
        buttonPushed = false;
    }
}


//-----------------------------------------------------------------------------
// GROUP1_IRQHandler
//
// Handle the falling edge interrupt on GDO0 and on S2 button (pushed)
//
// Input Parameters
//
//  - None
//
// Return
//
//  - None
//-----------------------------------------------------------------------------
void GROUP1_IRQHandler(void)
{
    // Get the pending interrupt for the GPIOB port and store for comparisons later
    uint32_t gpioB = DL_GPIO_getEnabledInterruptStatus(GPIOB, GPIO_BUTTON_S2_PIN |
                                                              GPIO_INPUT_GDO0_PIN);

    // Bitwise AND the pending interrupt with the pin you want to check, then check if it is
    // equal to the pins. Clear the interrupt status.
    if ((gpioB & GPIO_BUTTON_S2_PIN) == GPIO_BUTTON_S2_PIN)
    {
        buttonPushed = true;
        DL_GPIO_clearInterruptStatus(GPIOB, GPIO_BUTTON_S2_PIN);
    }

    if ((gpioB & GPIO_INPUT_GDO0_PIN) == GPIO_INPUT_GDO0_PIN)
    {
        packetSent = true;
        DL_GPIO_clearInterruptStatus(GPIOB, GPIO_INPUT_GDO0_PIN);
    }
}


//-----------------------------------------------------------------------------
// initMCU
//
// Initialize MCU and board peripherals
//
// Input Parameters
//
//  - None
//
// Return
//
//  - None
//-----------------------------------------------------------------------------
static void initMCU(void)
{
    // Perform any initialization needed before using any board APIs
    // (including SPI)
    SYSCFG_DL_init();

    NVIC_ClearPendingIRQ(SPI_0_INST_INT_IRQN);
    NVIC_EnableIRQ(SPI_0_INST_INT_IRQN);

    // Enable interrupt when SPI has finished transfers and changed into IDLE mode
    DL_SPI_clearInterruptStatus(SPI_0_INST, DL_SPI_INTERRUPT_IDLE);
    DL_SPI_enableInterrupt(SPI_0_INST, DL_SPI_INTERRUPT_IDLE);
}


//-----------------------------------------------------------------------------
// registerConfig
//
// Write register settings as given by SmartRF Studio found in
// smartrf_sesttings.h
//
// Input Parameters
//
//  - None
//
// Return
//
//  - None
//-----------------------------------------------------------------------------
static void registerConfig(void)
{
    uint8_t writeByte;

    #ifdef PA_TABLE
        uint8_t paTable[] = PA_TABLE;
    #endif

    // Reset radio
    mspm0SpiCmdStrobe(CC11xL_SRES);

    // Write registers to radio
    for(uint16_t i = 0; i < (sizeof(preferredSettings)/sizeof(registerSetting_t)); i++)
    {
        writeByte = preferredSettings[i].data;
        cc11xLSpiWriteReg(preferredSettings[i].addr, &writeByte, 1);
    }

    #ifdef PA_TABLE

        // write PA_TABLE
        cc11xLSpiWriteReg(CC11xL_PA_TABLE0, paTable, sizeof(paTable));
    #endif
}

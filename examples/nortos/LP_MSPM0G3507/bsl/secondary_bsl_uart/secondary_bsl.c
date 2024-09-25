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

#include "boot_config.h"
#include "factory_config.h"
#include "ti_msp_dl_config.h"

#include "bsl_ci.h"
#ifdef UART_Plugin
#include "bsl_uart.h"
#endif
#ifdef I2C_Plugin
#include "bsl_i2c.h"
#endif
#ifdef SPI_Plugin
#include "bsl_spi.h"
#endif
#ifdef CAN_Plugin
#include "bsl_mcan.h"
#endif
/* Global variables */

BSLPluginInfo *activePlugin;
uint8_t BSLLockedStatus;
uint16_t BSL_maxBufferSize2;
uint8_t *BSL_RXBuf;
uint8_t *BSL_TXBuf;
uint16_t pluginVersion;

uint32_t BSL_RAM_bufStartAddress;
uint32_t BSL_RAM_bufEndAddress;

volatile uint8_t timeoutDetected = (uint8_t) 0x0;

/* Static Function declaration */

/*
 * @brief       Timer interrupt handler
 */
void BSL_GPTimer_ISR(void);
/*
 * @brief       Initializes all the modules needed for BSL operation
 */
void BSL_SystemInit(void);
/*
 * @brief       Initializes the RAM memory used for Data buffer
 */
void BSL_RAM_BufferInit(void);
/*
 * @brief       Configures CRC module
 *
 * Configuration:
 * 1. CRC32-ISO3309 polynomial
 * 2. Bit reversed configuration
 * 3. Initial seed - 0xFFFFFFFF
 */
extern void BSL_CRC_Init(void);

/*
 * @brief      Configures timer
 *
 * Initializes the timer in One-shot mode and sets timer value for 10 seconds.
 * This is used to calculate timeout events in command reception phase
 */
extern void BSL_Timer_Init(void);

/*
 * @brief       Configures Flash Controller
 */
extern void BSL_Flash_Init(void);

/* Extern functions */
/*
 * @brief       The first function to get executed in the secondary BSL code
 */
extern void Reset_Handler(void);

/* The BSL and BCRConfig structures are defined in boot_config.c */
volatile BSL_Config *pBSLConfig = (volatile BSL_Config *) &BSLConfig;
volatile BCR_Config *pBCRConfig = (volatile BCR_Config *) &BCRConfig;
#ifdef UART_Plugin
#define BSL_SRAM_BUF_START_ADDR ((uint32_t) 0x20200120)
const BSLPluginInfo Interface = {
    .plugin_type = BSL_CFG_PLUGIN_TYPE_UART,
    .init        = BSL_PI_UART_init,
    .receive     = BSL_PI_UART_receive,
    .send        = BSL_PI_UART_send,
    .deinit      = BSL_PI_UART_deinit,
};
#endif

#ifdef I2C_Plugin
#define BSL_SRAM_BUF_START_ADDR ((uint32_t) 0x20200120)
const BSLPluginInfo Interface = {
    .plugin_type = BSL_CFG_PLUGIN_TYPE_I2C,
    .init        = BSL_PI_I2C_init,
    .receive     = BSL_PI_I2C_receive,
    .send        = BSL_PI_I2C_send,
    .deinit      = BSL_PI_I2C_deinit,
};
#endif

#ifdef SPI_Plugin
#define BSL_SRAM_BUF_START_ADDR ((uint32_t) 0x20200120)
const BSLPluginInfo Interface = {
    .plugin_type = BSL_CFG_PLUGIN_TYPE_ANY,
    .init        = BSL_PI_SPI_init,
    .receive     = BSL_PI_SPI_receive,
    .send        = BSL_PI_SPI_send,
    .deinit      = BSL_PI_SPI_deinit,
};
#endif

#ifdef CAN_Plugin
const BSLPluginInfo Interface = {
#define BSL_SRAM_BUF_START_ADDR ((uint32_t) 0x20200170)
    .plugin_type = BSL_CFG_PLUGIN_TYPE_ANY,
    .init        = BSL_PI_MCAN_init,
    .receive     = BSL_PI_MCAN_receive,
    .send        = BSL_PI_MCAN_send,
    .deinit      = BSL_PI_MCAN_deinit,
};
#endif

/**
 * @brief       BSL main function.
 * Handles initialization and runs the command interpreter as super loop.
 */
int main(void)
{
    uint32_t BSL_RXBufAddr;

    /*
     * Lock the BSL to prevent accessing any protected commands without
     * authentication
     */
    BSLLockedStatus = BSL_STATUS_LOCKED;

    /* Storing the UART as the active interface */
    activePlugin = (BSLPluginInfo *) &Interface;

    /* System Initialization */
    BSL_SystemInit();

    /* Initialize the Interface that will be used for BSL communication */
    pluginVersion = activePlugin->init(BSL_RXBuf, BSL_maxBufferSize2);

    /* Start the timer that creates timeout event after 10s */
    timeoutDetected = (uint8_t) 0x0;
    DL_TimerG_setLoadValue(BSL_TIMER, BSL_TEN_SECOND_TIMEOUT);
    DL_TimerG_startCounter(BSL_TIMER);

    /* Command reception phase */
    while (1) {
        /*
         * If no command is received for 10s and timeout is detected,
         * Stop the counter and enter low power mode to save power.
         * Also Lock the BSL before Sleep.
         */
        if (timeoutDetected == (uint8_t) 0x1) {
            DL_TimerG_stopCounter(BSL_TIMER);
            timeoutDetected = (uint8_t) 0x0;

            /*
             * Device goes into sleep mode.
             * It wakes up when a data is received over the active interface
             */
            DL_SYSCTL_setPowerPolicyRUN0SLEEP0();
            __WFI();

            /* Once the device wakes up, it executes instruction from here */

            /* Restart the timer after coming out of Sleep mode */
            DL_TimerG_startCounter(BSL_TIMER);
        }

        /*
         * Interface receive function is called to check if a valid command
         * packet is received. If so packet start address is returned.
         * Otherwise it returns '0'
         */
        BSL_RXBufAddr = activePlugin->receive();

        /*
         * If valid packet is received, process the command
         * and restart the timer
         */
        if (BSL_RXBufAddr != (uint32_t) 0x0) {
            /*
             * After processing the Data in the RXBuf the same buffer is
             * used as TXBuf to hold the BSL core response
             */
            BSL_RXBuf = (uint8_t *) BSL_RXBufAddr;
            BSL_TXBuf = BSL_RXBuf;

            /*
             * Stop the timer, Reset the flag
             * Restart the timer
             */
            DL_TimerG_stopCounter(BSL_TIMER);
            timeoutDetected = (uint8_t) 0x0;
            DL_TimerG_setLoadValue(BSL_TIMER, BSL_TEN_SECOND_TIMEOUT);
            DL_TimerG_startCounter(BSL_TIMER);

            /* Process the received command */
            BSL_CI_interpretCommand();
        }
    }
    return 0;
}

void BSL_SystemInit(void)
{
    BSL_RAM_BufferInit();
    BSL_CRC_Init();
    BSL_Timer_Init();
    BSL_Flash_Init();
}

void BSL_RAM_BufferInit(void)
{
    uint32_t *RAM_clearHandle;
    uint32_t RAM_available;

    /* Calculating the available SRAM size */
    /*
     * 'BSL_RAM_bufStartAddress' and  'BSL_RAM_bufEndAddress' gives the memory
     * range available for buffer operations, excluding the RAM consumed by
     * the Secondary BSL for it's operation
     */
    BSL_RAM_bufStartAddress = BSL_SRAM_BUF_START_ADDR;
    BSL_RAM_bufEndAddress   = BSL_CI_getRAMEndAddress() - BSL_STACK_SIZE;

    /*
     * Divide the available memory by 2,
     * since 2 buffers should be allocated in RAM
     */
    RAM_available =
        (BSL_RAM_bufEndAddress - BSL_RAM_bufStartAddress) / (uint32_t) 2;

    /*
     * Buffer size used for BSL communication is limited by the
     * 'BSL_MAX_BUF_SIZE_ALLOWED'
     */
    if (RAM_available > BSL_MAX_BUF_SIZE_ALLOWED) {
        BSL_maxBufferSize2 = (uint16_t) BSL_MAX_BUF_SIZE_ALLOWED;
    } else {
        BSL_maxBufferSize2 = (uint16_t) RAM_available;
    }

    /* Assign the buffer start address to RXBuffer */
    BSL_RXBuf = (uint8_t *) BSL_RAM_bufStartAddress;

    /* Clear the RAM used for data buffer */
    for (RAM_clearHandle = (uint32_t *) BSL_RAM_bufStartAddress;
         RAM_clearHandle < (uint32_t *) BSL_RAM_bufEndAddress;
         RAM_clearHandle++) {
        *RAM_clearHandle = (uint32_t) 0;
    }
}

void BSL_CRC_Init(void)
{
    /* Reset and enable power to CRC */
    DL_CRC_reset(BSL_CRC);
    DL_CRC_enablePower(BSL_CRC);
    while (DL_CRC_isPowerEnabled(BSL_CRC) != true) {
    }

    /* Initialize the CRC module */
    DL_CRC_init(BSL_CRC, DL_CRC_32_POLYNOMIAL, DL_CRC_BIT_REVERSED,
        DL_CRC_INPUT_ENDIANESS_LITTLE_ENDIAN, DL_CRC_OUTPUT_BYTESWAP_DISABLED);
}

void BSL_Timer_Init(void)
{
    DL_TimerG_TimerConfig timerConf;
    DL_TimerG_ClockConfig clockConf;

    timerConf.period       = BSL_TEN_SECOND_TIMEOUT;
    timerConf.timerMode    = DL_TIMER_TIMER_MODE_ONE_SHOT;
    timerConf.startTimer   = false;
    timerConf.counterVal   = (uint16_t) 0x0U;
    timerConf.genIntermInt = DL_TIMER_INTERM_INT_DISABLED;

    clockConf.clockSel    = DL_TIMER_CLOCK_LFCLK;
    clockConf.prescale    = BSL_TIMER_PRE_SCALE;
    clockConf.divideRatio = DL_TIMER_CLOCK_DIVIDE_1;

    /* Reset and enable power to peripheral*/
    DL_TimerG_reset(BSL_TIMER);
    DL_TimerG_enablePower(BSL_TIMER);

    /* Configure the timer */
    DL_TimerG_initTimerMode(BSL_TIMER, &timerConf);
    DL_TimerG_setClockConfig(BSL_TIMER, &clockConf);

    DL_TimerG_enableClock(BSL_TIMER);

    DL_TimerG_enableInterrupt(BSL_TIMER, DL_TIMER_INTERRUPT_ZERO_EVENT);

    /* Enable the interrupt */
    DL_Interrupt_registerInterrupt(
        (uint32_t) BSL_GPTIMER_INT_VEC, BSL_GPTimer_ISR);
    NVIC_EnableIRQ(BSL_GPTIMER_INT_VEC);
}

void BSL_Flash_Init(void)
{
    /* Set flash wait state to 2 based on 32MHz clock*/
    DL_FlashCTL_setWaitState(FLASHCTL, (uint32_t) 2);
}

void BSL_GPTimer_ISR(void)
{
    /*
     * Whenever timer interrupt is triggered, it sets the 'timeoutDetected'
     * flag, which will be checked in the infinite loop that takes the
     * necessary action
     */
    timeoutDetected = (uint8_t) 0x1;
}

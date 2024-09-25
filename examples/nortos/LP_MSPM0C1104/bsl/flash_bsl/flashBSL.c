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

#include <flashBSL_ci.h>
#include <flashBSL_defines.h>
#include <flashBSL_i2c.h>
#include <flashBSL_invocation.h>
#include <flashBSL_uart.h>
#include "boot_config.h"

/* Global variables */
uint8_t BSLLockedStatus;
uint16_t BSL_maxBufferSize2;
uint8_t *BSL_RXBuf;
uint8_t *BSL_TXBuf;
uint16_t pluginVersion;

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
 * @brief       Initializes the User specified features
 */
void BSL_User_Init(void);
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

/* The BSL and BCRConfig structures are defined in boot_config.c */
volatile BSL_Config *pBSLConfig = (volatile BSL_Config *) &BSLConfig;
volatile BCR_Config *pBCRConfig = (volatile BCR_Config *) &BCRConfig;

int main(void)
{
    uint32_t BSL_RXBufAddr;

    /******** ROM BSL Init phase *********/

    /*
     * Lock the BSL to prevent accessing any protected commands without
     * authentication
     */
    BSLLockedStatus = BSL_STATUS_LOCKED;

    /* System Initialization */
    BSL_SystemInit();

    /* Initialize the Interface that will be used for BSL communication */
    pluginVersion = ACTIVE_INTERFACE_INIT(BSL_RXBuf, BSL_maxBufferSize2);

#if BSL_TIMEOUT
    /* Start the timer that creates timeout event after 10s */
    DL_TimerG_stopCounter(BSL_TIMER);
    timeoutDetected = (uint8_t) 0x0;
    DL_TimerG_setLoadValue(BSL_TIMER, BSL_TEN_SECOND_TIMEOUT);
    DL_TimerG_startCounter(BSL_TIMER);
    /* Command reception phase */
#endif
    while (1) {
#if BSL_TIMEOUT
        /*
         * If no command is received for 10s and timeout is detected,
         * Stop the counter and enter low power mode to save power.
         * Also Lock the BSL before Sleep.
         */
        if (timeoutDetected == (uint8_t) 0x1) {
            DL_TimerG_stopCounter(BSL_TIMER);
            timeoutDetected = (uint8_t) 0x0;
            BSLLockedStatus = BSL_STATUS_LOCKED;
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
#endif

        /*
         * Interface receive function is called to check if a valid command
         * packet is received. If so packet start address is returned.
         * Otherwise it returns '0'
         */
        BSL_RXBufAddr = ACTIVE_INTERFACE_RECEIVE();

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
#if BSL_TIMEOUT
            /*
             * Stop the timer, Reset the flag
             * Restart the timer
             */
            DL_TimerG_stopCounter(BSL_TIMER);
            timeoutDetected = (uint8_t) 0x0;
            DL_TimerG_setLoadValue(BSL_TIMER, BSL_TEN_SECOND_TIMEOUT);
            DL_TimerG_startCounter(BSL_TIMER);
#endif
            /* Process the received command */
            BSL_CI_interpretCommand();
        }
    }
    return 0;
}
void BSL_SystemInit(void)
{
    BSL_User_Init();
    BSL_RAM_BufferInit();
    BSL_CRC_Init();
#if BSL_TIMEOUT
    BSL_Timer_Init();
#endif
    BSL_Flash_Init();
}
void BSL_User_Init(void)
{
    /*
     * Some low pin count devices support reconfiguring the NRST pin to be a GPIO pin.
     * See the pin configuration of the device-specific data sheet to see if
     * GPIO functionality is shared with NRST.
     * Application software can disable the NRST functionality of the NRST pin,
     * allowing GPIO functionality to be enabled.
     */
    if (pBSLConfig->disableNRST == BL_CFG_NRST_DIS) {
        DL_SYSCTL_disableNRSTPin();
    }
}
void BSL_RAM_BufferInit(void)
{
    uint32_t *RAM_clearHandle;

    /* Calculating the available SRAM size */
    /*
     * 'BSL_RAM_bufStartAddress' and  'BSL_RAM_bufEndAddress' gives the memory
     * range available for buffer operations
     */

    /*
     * Divide the available memory by 2,
     * since 2 buffers should be allocated in RAM
     */
    BSL_maxBufferSize2 =
        (BSL_SRAM_BUF_END_ADDR - BSL_SRAM_BUF_START_ADDR) / (uint32_t) 2;

    /* Assign the buffer start address to RXBuffer */
    BSL_RXBuf = (uint8_t *) BSL_SRAM_BUF_START_ADDR;

    /* Clear the RAM used for data buffer */
    for (RAM_clearHandle = (uint32_t *) BSL_SRAM_BUF_START_ADDR;
         RAM_clearHandle < (uint32_t *) BSL_SRAM_BUF_END_ADDR;
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
    DL_CRC_init(BSL_CRC, DL_CRC_POLYNOMIAL_TYPE, DL_CRC_BIT_REVERSED,
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

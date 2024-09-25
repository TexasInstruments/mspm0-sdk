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
#include "i2c_target.h"

/*====================== TYPEDEFS/DEFINES/MACROS ===========================*/
enum buttonStates
{
    eReleased,
    ePressed,
};

typedef struct
{
    uint8_t bStateChange: 1;
    uint8_t bState: 1;
}tButton;

/*============================ VARIABLES ===================================*/
tButton Button1 = {eReleased};
tButton Button2 = {eReleased};

volatile bool bTMGBusyFlag;
volatile bool bIRQFlag;

/*====================== FORWARD DECLARATIONS ==============================*/
void msecDelayBlocking(uint32_t msec);
void notifyHost(void);
void checkButtons(void);

/*============================ FUNCTIONS ===================================*/
int main(void)
{
    SYSCFG_DL_init();

    NVIC_EnableIRQ(I2C_TARGET_INST_INT_IRQN);
    NVIC_EnableIRQ(TIMG_DELAY_BLOCKING_INST_INT_IRQN);
    NVIC_EnableIRQ(GPIO_A_INT_IRQN);
    NVIC_EnableIRQ(GPIO_B_INT_IRQN);

    /* Allow CPU to process after each wakeup event */
    DL_SYSCTL_disableSleepOnExit();

    while (1)
    {
        /* Remain in low power mode until interrupt wakes CPU */
        __WFI();


        /* =========================================================================
         * Test for messages received.
         * =========================================================================
         */
        if(g_bMessageReady == true)
        {
            g_bMessageReady = false;
            processMessage();
        }
        checkButtons();
    }
}

/* =========================================================================
 * Test for button presses
 * Button1 = S2 (PA18) - not used in this example
 * Button2 = S3 (PB21) USER - sends interrupt to host MCU
 * Note: GPIO Input filter is enabled which provides debounce (8 ULPCLK cycles).
 * =========================================================================
 */
void checkButtons(void)
{
        /* SW_S2 */
        if(Button1.bStateChange == true)
        {
            if(Button1.bState == ePressed)
            {
                /* Not used in this example.
                 * Add some code to do something useful.
              	 */
            }

            Button1.bStateChange = false;
        }

        /* S3 - USER */
        if(Button2.bStateChange == true)
        {
            if(Button2.bState == ePressed)
            {
                /* Generate an external interrupt on host (controller) MCU */
                notifyHost();
            }

            Button2.bStateChange = false;
        }
}

/* GPIO interrupt handling example */
void GROUP1_IRQHandler(void)
{
    switch(DL_GPIO_getPendingInterrupt(GPIO_A_PORT))
    {
        /* S2 is pulled low with a resistor and goes high when pressed.
         * Although not used in this example, example handler is provided
         */
        case GPIO_A_SW_S2_IIDX:

            Button1.bStateChange = true;

            if(DL_GPIO_readPins(GPIO_A_PORT, GPIO_A_SW_S2_PIN) == GPIO_A_SW_S2_PIN)
            {
                /* SW_S1 is '1' */
                Button1.bState = ePressed;
            }
            else
            {
                /* SW_S1 is '0' */
                Button1.bState = eReleased;
            }
            break;

        default:
            break;
    }

    switch(DL_GPIO_getPendingInterrupt(GPIO_B_PORT))
    {
        /* S3 is high and goes low when pressed */
        case GPIO_B_SW_S3_IIDX:

            Button2.bStateChange = true;

            if (DL_GPIO_readPins(GPIO_B_PORT, GPIO_B_SW_S3_PIN) == GPIO_B_SW_S3_PIN)
            {
                /* SW_S3 = 1 = released*/
                DL_GPIO_clearPins(GPIO_B_PORT, GPIO_B_LED_GREEN_PIN);

                Button2.bState = eReleased;
            }
            else
            {
                /* SW_S3 = 0 = pressed - Clear all RGB LEDs first */
                DL_GPIO_clearPins(GPIO_B_PORT, GPIO_B_LED_RED_PIN |
                                 GPIO_B_LED_GREEN_PIN |
                                 GPIO_B_LED_BLUE_PIN);

                DL_GPIO_setPins(GPIO_B_PORT, GPIO_B_LED_GREEN_PIN);

                Button2.bState = ePressed;
            }
            break;

        default:
            break;
    }

}

/*
 * Interrupt handler for one-shot delay timer.
 * Timer halted automatically
 * Just clear the busy flag and return.
 */
void TIMG_DELAY_BLOCKING_INST_IRQHandler(void)
{
    switch (DL_TimerG_getPendingInterrupt(TIMG_DELAY_BLOCKING_INST))
    {
        case DL_TIMER_IIDX_ZERO:
            bTMGBusyFlag = false;
            break;

        default:
            break;
    }
}

/*
 * A programmable (msec) timer delay (one-shot, blocking)
 * In this example, it is used to generate a 1msec pulse.
 *
 * Note: uses LFCLK, which is 32.768kHz +/-3%
 * Possible you may see delays that are +/-3%
 * Argument is shifted by 32 to convert to ~msec
 *
 */
void msecDelayBlocking(uint32_t msec)
{
    /* reconfigure timer for new period */
    DL_TimerG_stopCounter(TIMG_DELAY_BLOCKING_INST);
    DL_TimerG_setLoadValue(TIMG_DELAY_BLOCKING_INST, (msec << 5));
    DL_TimerG_startCounter(TIMG_DELAY_BLOCKING_INST);
    bTMGBusyFlag = true;

    while(bTMGBusyFlag == true)
    {
        __WFI();
    }
}

/*
 * GPIO_B_IRQ_OUT_PIN
 * In Sysconf, pin is configured as high-z output.
 * Internal pull up resistors keep pin high. This
 * emulates an open-drain configuration.
 *
 * Active state = Pin driven low for 1msec
 * Inactive state = Pulled to high state
 */
inline void notifyHost(void)
{
    /* Active pull low */
    DL_GPIO_clearPins( GPIOB, GPIO_B_IRQ_OUT_PIN);

    msecDelayBlocking(1);

    /* Hi-Z output uses pull up resistor to go high */
    DL_GPIO_setPins( GPIOB, GPIO_B_IRQ_OUT_PIN);
}

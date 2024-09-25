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
#include "demo.h"

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
tButton Button2 = {false, eReleased};
tButton Button3 = {false, eReleased};

/* Millisecond delay timer busy flag */
volatile bool bTMGBusyFlag;

/* External interrupt flag */
volatile bool bIRQFlag;

/*====================== FORWARD DECLARATIONS ==============================*/
void msecDelayBlocking(uint32_t msec);
void msecDelayNoBlocking(uint32_t msec);
void checkButtons(void);

/*============================ FUNCTIONS ===================================*/
int main(void)
{
    SYSCFG_DL_init();

    NVIC_EnableIRQ(I2C_CONTROLLER_INST_INT_IRQN);
    NVIC_EnableIRQ(TIMG_DELAY_NO_BLOCKING_INST_INT_IRQN);
    NVIC_EnableIRQ(TIMG_DELAY_BLOCKING_INST_INT_IRQN);
    NVIC_EnableIRQ(GPIO_A_INT_IRQN);
    NVIC_EnableIRQ(GPIO_B_INT_IRQN);

    DL_SYSCTL_disableSleepOnExit();

    while (1)
    {
        __WFI();

        checkButtons();
    }
}

/****************************************************************************
 * Test for button presses
 * Button2 = S2 (PA18) - not used in this example
 * Button3 = S3 (PB21) USER - steps through demo sequence
 * Note: GPIO Input filter enabled = debounce (8 ULPCLK cycles).
 * This is selected in the Sysconfig - GPIO Digital IOMUX Features.
 ****************************************************************************/
void checkButtons(void)
{
    /* SW_S2 */
    if(Button2.bStateChange == true)
    {
        if(Button2.bState == ePressed)
        {
            /* Add some code to do something useful */
        }
        Button2.bStateChange = false;
    }

    /* SW_S3 - USER */
    if(Button3.bStateChange == true)
    {
        if(Button3.bState == ePressed)
        {
            demoIOx();
        }
        Button3.bStateChange = false;
    }

}

/* Example GPIO interrupt handler.
 * Handles SW2 and SW3 button presses.
 */
void GROUP1_IRQHandler(void)
{
    /* SW_S2 Handler */
    switch(DL_GPIO_getPendingInterrupt(GPIO_A_PORT))
    {
        /* S2 is pulled low with a resistor and goes high when pressed.
         * Although not used in this example, example handler is provided here.
         */
        case GPIO_A_SW_S2_IIDX:

            /* Set flag to indicate the button state has changed */
            Button2.bStateChange = true;

            if(DL_GPIO_readPins(GPIO_A_PORT, GPIO_A_SW_S2_PIN) == GPIO_A_SW_S2_PIN)
            {
                /* SW_S2 = 1 = released */
                Button2.bState = eReleased;
            }
            else
            {
                /* SW_S2 = 0 = pressed */
                Button2.bState = ePressed;
            }
            break;

        default:
            break;
    }

    /* SW_S3 and IRQ_IN share handler */
    switch(DL_GPIO_getPendingInterrupt(GPIO_B_PORT))
    {
        /* Interrupt from IO Expander MCU */
        case GPIO_B_IRQ_IN_IIDX:
            {
                /* Leave the LED on for 100msec - non-blocking */
                msecDelayNoBlocking(100);
            }
            break;

            /* SW3 - USER */
        case GPIO_B_SW_S3_USER_IIDX:

            /* Button state has changed due to either rising or falling edges */
            Button3.bStateChange = true;

            /* Read the current state of SW3 */
            if (DL_GPIO_readPins(GPIO_B_PORT, GPIO_B_SW_S3_USER_PIN) == GPIO_B_SW_S3_USER_PIN)
            {
                /* SW_S3 = 1 = released */
                DL_GPIO_clearPins(GPIO_B_PORT, GPIO_B_LED_RGB_GREEN_PIN);

                Button3.bState = eReleased;

            }
            else
            {
                /* SW_S3 = 0 = pressed */
                DL_GPIO_setPins(GPIO_B_PORT, GPIO_B_LED_RGB_GREEN_PIN);

                Button3.bState = ePressed;
            }
        default:
            break;
    }
}

/*
 * Interrupt handler for one-shot (blocking) delay timer.
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
 * Interrupt handler for one-shot (non-blocking)
 */
void TIMG_DELAY_NO_BLOCKING_INST_IRQHandler(void)
{
    switch (DL_TimerG_getPendingInterrupt(TIMG_DELAY_NO_BLOCKING_INST))
        {
            case DL_TIMER_IIDX_ZERO:
                DL_GPIO_clearPins(GPIO_B_PORT, GPIO_B_LED_RGB_BLUE_PIN);
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
 * A programmable (msec) timer delay (non-blocking)
 * Used to create a delay
 *
 * Note: uses LFCLK, which is 32.768kHz +/-3%
 * Possible you may see delays that are +/-3%
 *
 */
void msecDelayNoBlocking(uint32_t msec)
{
    /* reconfigure timer for new period */
    DL_TimerG_stopCounter(TIMG_DELAY_NO_BLOCKING_INST);
    DL_TimerG_setLoadValue(TIMG_DELAY_NO_BLOCKING_INST, (msec << 5));
    DL_TimerG_startCounter(TIMG_DELAY_NO_BLOCKING_INST);
}

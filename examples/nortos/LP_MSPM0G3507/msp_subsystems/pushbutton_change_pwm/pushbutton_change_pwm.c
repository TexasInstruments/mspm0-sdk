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
/*
 * READ ME
 * This example allows for the user to edit the PWM signal based on button presses.
 * If the user presses switch three, the duty cycle will change. If switch two is
 * pressed, the period will change.  The PWM signal is displayed on GPIO PIN B15.
 * There are four unique duty cycles which the PWM signal can have, as well as
 * four unique periods.  The duty cycles are at 50%, 25%, 10%, and 75%, and at
 * default, the duty cycle is at 50%.  If the user updates the period after updating
 * the duty cycle, the duty cycle will keep at whatever percentage it has been set to.
 */
#include "ti_msp_dl_config.h"
/*
 * This example uses TIMA1 to generate two PWM waveforms.
 * Pressing switch S1 changes the PWM period to one of four define periods.
 * Pressing switch S2 changes the duty cycle to one of four defined duty cycles.
 */

/* Initial PWM period */
#define PWM_PERIOD              (1000)

/* Initialize variable which sets the period */
volatile uint32_t gPeriodVal = PWM_PERIOD;

/* Keeps track of state of period in case structure */
uint8_t  periodState = 0;

/* Keeps track of state of duty cycle in case structure */
uint8_t  dutyCycleState = 0;

/* Initialize variable which stores duty cycle to half of default period */
uint32_t dutyCycle = 500;

/* Software flag edited by interrupt handler to change state of period */
volatile uint8_t swFlagPeriod = 0;

/* Software flag edited by interrupt handler to change state of duty cycle */
volatile uint8_t swFlagDutyCycle = 0;

/* Software flag indicating which switch was pressed */
uint32_t swFlagSwitchPressed = 0;

int main(void)
{
    SYSCFG_DL_init();

    DL_SYSCTL_disableSleepOnExit();
    NVIC_EnableIRQ(GPIO_SWITCHES_GPIOA_INT_IRQN);
    NVIC_EnableIRQ(GPIO_SWITCHES_GPIOB_INT_IRQN);
    NVIC_EnableIRQ(TIMER_INST_INT_IRQN);
    DL_TimerG_startCounter(PWM_INST);

    while (1) {
#if 1
        /* Only enter switch structure when software flag
         * has been set for the period */
        if (swFlagPeriod){
            switch (periodState) {

                case 0:
                    /* Period and duty cycle x2 */
                    gPeriodVal = PWM_PERIOD * 2;
                    dutyCycle = dutyCycle*2;
                    DL_TimerG_setLoadValue(PWM_INST, gPeriodVal);
                    DL_TimerG_setCaptureCompareValue(
                            PWM_INST, dutyCycle, DL_TIMERG_CAPTURE_COMPARE_0_INDEX);
                    periodState = 1;
                    swFlagPeriod = 0;
                    break;

                case 1:
                    /* Period and duty cycle x4 */
                    gPeriodVal = PWM_PERIOD * 4;
                    dutyCycle = dutyCycle*2;
                    DL_TimerG_setLoadValue(PWM_INST, gPeriodVal);
                    DL_TimerG_setCaptureCompareValue(
                            PWM_INST, dutyCycle, DL_TIMERG_CAPTURE_COMPARE_0_INDEX);
                    periodState = 2;
                    swFlagPeriod = 0;
                    break;

                case 2:
                    /* Period and duty cycle x0.5 */
                    gPeriodVal = PWM_PERIOD * 0.5;
                    dutyCycle = dutyCycle/8;
                    DL_TimerG_setLoadValue(PWM_INST, gPeriodVal);
                    DL_TimerG_setCaptureCompareValue(
                            PWM_INST, dutyCycle, DL_TIMERG_CAPTURE_COMPARE_0_INDEX);
                    periodState = 3;
                    swFlagPeriod = 0;
                    break;

                case 3:
                    /* Period and duty cycle to default */
                    gPeriodVal = PWM_PERIOD;
                    dutyCycle = dutyCycle*2;
                    DL_TimerG_setLoadValue(PWM_INST, gPeriodVal);
                    DL_TimerG_setCaptureCompareValue(
                            PWM_INST, dutyCycle, DL_TIMERG_CAPTURE_COMPARE_0_INDEX);
                    periodState = 0;
                    swFlagPeriod = 0;
                    break;
                default:
                    break;
                }

        } else if (swFlagDutyCycle){

            switch (dutyCycleState){

                case 0:
                    /* Change duty cycle to 25% */
                    dutyCycle = 0.75*gPeriodVal;
                    DL_TimerG_setCaptureCompareValue(
                            PWM_INST, dutyCycle, DL_TIMERG_CAPTURE_COMPARE_0_INDEX);
                    dutyCycleState = 1;
                    swFlagDutyCycle = 0;
                    break;
                case 1:
                    /* Change duty cycle to 10% */
                    dutyCycle = 0.9*gPeriodVal;
                    DL_TimerG_setCaptureCompareValue(
                            PWM_INST, dutyCycle, DL_TIMERG_CAPTURE_COMPARE_0_INDEX);
                    dutyCycleState = 2;
                    swFlagDutyCycle = 0;
                    break;
                case 2:
                    /* Change duty cycle to 75% */
                    dutyCycle = 0.25*gPeriodVal;
                    DL_TimerG_setCaptureCompareValue(
                            PWM_INST, dutyCycle, DL_TIMERG_CAPTURE_COMPARE_0_INDEX);
                    dutyCycleState = 3;
                    swFlagDutyCycle = 0;
                    break;
                case 3:
                    /* Change duty cycle to 50% */
                    dutyCycle = 0.5*gPeriodVal;
                    DL_TimerG_setCaptureCompareValue(
                            PWM_INST, dutyCycle, DL_TIMERG_CAPTURE_COMPARE_0_INDEX);
                    dutyCycleState = 0;
                    swFlagDutyCycle = 0;
                    break;
                default:
                    break;

                            }
        } else{
            __WFI();
        }
#endif
    }

}

/*
 * Switch Interrupt Handler
 * Both GPIO ports share same handler.
 * Check each port's pending interrupt, then IO pin index
 */
void GROUP1_IRQHandler(void)
{
    switch (DL_GPIO_getPendingInterrupt(GPIO_SWITCHES_USER_SWITCH_1_PORT)){
        case GPIO_SWITCHES_USER_SWITCH_1_IIDX:

            DL_GPIO_togglePins(
                    GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);

            DL_GPIO_clearPins(
                    GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_2_PIN);

            DL_GPIO_clearInterruptStatus(
                    GPIOA, GPIO_SWITCHES_USER_SWITCH_1_PIN);

            /* Disable interrupt for switch debounce */
            NVIC_DisableIRQ(GPIO_SWITCHES_GPIOA_INT_IRQN);

            /* Start timer for which no interrupt can be thrown */
            DL_TimerG_startCounter(TIMER_INST);

            /* Set software flag for period */
            swFlagPeriod = 1;

            swFlagSwitchPressed = GPIO_SWITCHES_USER_SWITCH_1_IIDX;

            break;
        default:
            break;
    }

    switch(DL_GPIO_getPendingInterrupt(GPIO_SWITCHES_USER_SWITCH_2_PORT)){
        case GPIO_SWITCHES_USER_SWITCH_2_IIDX:

            DL_GPIO_togglePins(
                    GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_2_PIN);

            DL_GPIO_clearPins(
                    GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);

            DL_GPIO_clearInterruptStatus(
                    GPIOB, GPIO_SWITCHES_USER_SWITCH_2_PIN);

            /* Disable interrupt for switch debounce */
            NVIC_DisableIRQ(GPIO_SWITCHES_GPIOB_INT_IRQN);

            /* Start timer for which no interrupt can be thrown */
            DL_TimerG_startCounter(TIMER_INST);

            /* Set software flag for duty cycle */
            swFlagDutyCycle = 1;

            swFlagSwitchPressed = GPIO_SWITCHES_USER_SWITCH_2_IIDX;

            break;
        default:
            break;
    }
}


/*Switch Debounce*/

void TIMER_INST_IRQHandler(void)
{
    switch (DL_TimerG_getPendingInterrupt(TIMER_INST)) {
        case DL_TIMER_IIDX_ZERO:

            /* Test which switch interrupt is disabled and re-enable it here. */
            if(swFlagSwitchPressed == GPIO_SWITCHES_USER_SWITCH_1_IIDX)
                NVIC_EnableIRQ(GPIO_SWITCHES_GPIOA_INT_IRQN);

            else if(swFlagSwitchPressed == GPIO_SWITCHES_USER_SWITCH_2_IIDX)
                NVIC_EnableIRQ(GPIO_SWITCHES_GPIOB_INT_IRQN);

             break;
        default:
            break;

    }
}

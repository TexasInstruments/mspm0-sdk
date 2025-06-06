/*
 * Copyright (c) 2023, Texas Instruments Incorporated
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

class controlGPIOPin
{
   public:
    controlGPIOPin(GPIO_Regs* gpio, uint32_t pins)
    {
        gpioPort = gpio;
        gpioPins = pins;
    }
    void clear(void)
    {
        DL_GPIO_clearPins(gpioPort, gpioPins);
    }
    void set(void)
    {
        DL_GPIO_setPins(gpioPort, gpioPins);
    }
    void toggle(void)
    {
        DL_GPIO_togglePins(gpioPort, gpioPins);
    }

   private:
    GPIO_Regs* gpioPort;
    uint32_t gpioPins;
};

int main(void)
{
    controlGPIOPin gpioLED1(
        GPIO_LEDS_USER_LED_1_PORT, GPIO_LEDS_USER_LED_1_PIN);
    controlGPIOPin gpioLED2(
        GPIO_LEDS_USER_LED_2_PORT, GPIO_LEDS_USER_LED_2_PIN);
    controlGPIOPin gpioLED3(
        GPIO_LEDS_USER_LED_3_PORT, GPIO_LEDS_USER_LED_3_PIN);
    controlGPIOPin gpioUserTestPin(
        GPIO_LEDS_USER_TEST_PORT, GPIO_LEDS_USER_TEST_PIN);

    /* Power on GPIO, initialize pins as digital outputs */
    SYSCFG_DL_init();

    /* Default: LED1, LED3 and TestPin ON, LED2 OFF */
    gpioLED1.set();
    gpioLED2.clear();
    gpioLED3.set();
    gpioUserTestPin.set();

    __BKPT(0);

    while (1) {
        delay_cycles(CPUCLK_FREQ);
        gpioLED1.toggle();
        gpioLED2.toggle();
        gpioLED3.toggle();
        gpioUserTestPin.toggle();
    }
}

/*
 * Copyright (c) 2023, Texas Instruments Incorporated - http://www.ti.com
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
 *  ============ ti_drivers_config.c =============
 *  Configured MSPM0 Driver module declarations
 *
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */

#include <stddef.h>

#include "ti_drivers_config.h"

#include <ti/drivers/GPIO.h>
#include <ti/drivers/gpio/GPIOMSPM0.h>

/* The range of pins available on this device */
const uint_least8_t GPIO_pinLowerBound = 0;
const uint_least8_t GPIO_pinUpperBound = 60;

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

/*
 *  ======== gpioPinConfigs ========
 *  Array of Pin configurations
 */
/* The index denotes the pin number (index % 32) */
GPIO_PinConfig gpioPinConfigs[60] = {
    GPIO_DO_NOT_CONFIG, /* PA0 */
    GPIO_DO_NOT_CONFIG, /* PA1 */
    GPIO_DO_NOT_CONFIG, /* PA2 */
    GPIO_DO_NOT_CONFIG, /* PA3 */
    GPIO_DO_NOT_CONFIG, /* PA4 */
    GPIO_DO_NOT_CONFIG, /* PA5 */
    GPIO_DO_NOT_CONFIG, /* PA6 */
    GPIO_DO_NOT_CONFIG, /* PA7 */
    GPIO_DO_NOT_CONFIG, /* PA8 */
    GPIO_DO_NOT_CONFIG, /* PA9 */
    GPIO_DO_NOT_CONFIG, /* PA10 */
    GPIO_DO_NOT_CONFIG, /* PA11 */
    GPIO_DO_NOT_CONFIG, /* PA12 */
    GPIO_DO_NOT_CONFIG, /* PA13 */
    GPIO_DO_NOT_CONFIG, /* PA14 */
    GPIO_DO_NOT_CONFIG, /* PA15 */
    GPIO_DO_NOT_CONFIG, /* PA16 */
    GPIO_DO_NOT_CONFIG, /* PA17 */
    GPIO_DO_NOT_CONFIG, /* PA18 */
    GPIO_DO_NOT_CONFIG, /* PA19 */
    GPIO_DO_NOT_CONFIG, /* PA20 */
    GPIO_DO_NOT_CONFIG, /* PA21 */
    GPIO_DO_NOT_CONFIG, /* PA22 */
    GPIO_DO_NOT_CONFIG, /* PA23 */
    GPIO_DO_NOT_CONFIG, /* PA24 */
    GPIO_DO_NOT_CONFIG, /* PA25 */
    GPIO_DO_NOT_CONFIG, /* PA26 */
    GPIO_DO_NOT_CONFIG, /* PA27 */
    GPIO_DO_NOT_CONFIG, /* PA28 */
    GPIO_DO_NOT_CONFIG, /* PA29 */
    GPIO_DO_NOT_CONFIG, /* PA30 */
    GPIO_DO_NOT_CONFIG, /* PA31 */
    GPIO_DO_NOT_CONFIG, /* PB0 */
    GPIO_DO_NOT_CONFIG, /* PB1 */
    GPIO_DO_NOT_CONFIG, /* PB2 */
    GPIO_DO_NOT_CONFIG, /* PB3 */
    GPIO_DO_NOT_CONFIG, /* PB4 */
    GPIO_DO_NOT_CONFIG, /* PB5 */
    GPIO_DO_NOT_CONFIG, /* PB6 */
    GPIO_DO_NOT_CONFIG, /* PB7 */
    GPIO_DO_NOT_CONFIG, /* PB8 */
    GPIO_DO_NOT_CONFIG, /* PB9 */
    GPIO_DO_NOT_CONFIG, /* PB10 */
    GPIO_DO_NOT_CONFIG, /* PB11 */
    GPIO_DO_NOT_CONFIG, /* PB12 */
    GPIO_DO_NOT_CONFIG, /* PB13 */
    GPIO_DO_NOT_CONFIG, /* PB14 */
    GPIO_DO_NOT_CONFIG, /* PB15 */
    GPIO_DO_NOT_CONFIG, /* PB16 */
    GPIO_DO_NOT_CONFIG, /* PB17 */
    GPIO_DO_NOT_CONFIG, /* PB18 */
    GPIO_DO_NOT_CONFIG, /* PB19 */
    GPIO_DO_NOT_CONFIG, /* PB20 */
    GPIO_DO_NOT_CONFIG, /* PB21 */
    GPIO_DO_NOT_CONFIG, /* PB22 */
    GPIO_DO_NOT_CONFIG, /* PB23 */
    GPIO_DO_NOT_CONFIG, /* PB24 */
    GPIO_DO_NOT_CONFIG, /* PB25 */
    GPIO_DO_NOT_CONFIG, /* PB26 */
    GPIO_DO_NOT_CONFIG, /* PB27 */
};

/* This is an interrupt example, use callbacks */
GPIO_CallbackFxn gpioCallbackFunctions[60] = {0};

void *gpioUserArgs[60];

const GPIO_Config GPIO_config = {.configs = (GPIO_PinConfig *) gpioPinConfigs,
    .callbacks   = (GPIO_CallbackFxn *) gpioCallbackFunctions,
    .userArgs    = gpioUserArgs,
    .intPriority = (~0)};

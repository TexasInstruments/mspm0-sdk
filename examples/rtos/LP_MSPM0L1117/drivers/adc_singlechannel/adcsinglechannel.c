/*
 * Copyright (c) 2018-2024, Texas Instruments Incorporated
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
 *  ======== adcsinglechannel.c ========
 */
#include <stddef.h>
#include <stdint.h>

/* POSIX Header files */
#include <pthread.h>

/* Driver Header files */
#include <ti/display/DisplayUart.h>
#include <ti/drivers/ADC.h>
#include <ti/drivers/GPIO.h>

/* Driver configuration */
#include "ti_drivers_config.h"

/* ADC sample count */
#define ADC_SAMPLE_COUNT (10)

#define THREADSTACKSIZE (768)

/* ADC conversion result variables */
uint16_t adcValue0[ADC_SAMPLE_COUNT];
uint32_t adcValue0MicroVolt[ADC_SAMPLE_COUNT];

static Display_Handle display;

/*
 *  ======== threadFxn0 ========
 *  Open an ADC instance and get a sampling result from a one-shot conversion.
 */
void *threadFxn0(void *arg0)
{
    uint16_t i;
    ADC_Handle adc;
    ADC_Params params;
    int_fast16_t res;

    ADC_Params_init(&params);
    adc = ADC_open(CONFIG_ADC_0, &params);

    if (adc == NULL) {
        Display_printf(display, 0, 0, "Error initializing CONFIG_ADC_1\n");
        while (1) {
        }
    }

    for (i = 0; i < ADC_SAMPLE_COUNT; i++) {
        res = ADC_convert(adc, &adcValue0[i]);

        if (res == ADC_STATUS_SUCCESS) {
            adcValue0MicroVolt[i] = ADC_convertToMicroVolts(adc, adcValue0[i]);

            Display_printf(display, 0, 0, "CONFIG_ADC_0 raw result (%d): %d\n",
                i, adcValue0[i]);
            Display_printf(display, 0, 0,
                "CONFIG_ADC_0 convert result (%d): %d uV\n", i,
                adcValue0MicroVolt[i]);
        } else {
            Display_printf(
                display, 0, 0, "CONFIG_ADC_0 convert failed (%d)\n", i);
        }
    }

    ADC_close(adc);

    return (NULL);
}

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    pthread_t thread0, thread1;
    pthread_attr_t attrs;
    struct sched_param priParam;
    int retc;
    int detachState;

    /* Call driver init functions */
    Display_init();
    GPIO_init();
    ADC_init();

    /* Configure the LED and if applicable, the TMP_EN pin */
    GPIO_setConfig(CONFIG_GPIO_LED_0,
        GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH | CONFIG_GPIO_LED_0_IOMUX);
    /* Open the UART display for output */
    display = Display_open(Display_Type_UART, NULL);
    if (display == NULL) {
        /* Failed to open display driver */
        while (1) {
        }
    }

    /* Turn on user LED to indicate successful initialization */
    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_LED_ON);
    Display_printf(display, 0, 0, "Starting the adcsinglechannel example\n");

    /* Create application threads */
    pthread_attr_init(&attrs);

    detachState = PTHREAD_CREATE_DETACHED;
    /* Set priority and stack size attributes */
    retc = pthread_attr_setdetachstate(&attrs, detachState);
    if (retc != 0) {
        /* pthread_attr_setdetachstate() failed */
        while (1) {
        }
    }

    retc |= pthread_attr_setstacksize(&attrs, THREADSTACKSIZE);
    if (retc != 0) {
        /* pthread_attr_setstacksize() failed */
        while (1) {
        }
    }

    /* Create threadFxn0 thread */
    priParam.sched_priority = 1;
    pthread_attr_setschedparam(&attrs, &priParam);

    retc = pthread_create(&thread0, &attrs, threadFxn0, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1) {
        }
    }

    return (NULL);
}

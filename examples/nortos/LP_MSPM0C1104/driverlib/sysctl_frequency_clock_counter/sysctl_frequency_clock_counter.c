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

/* clang-format off */

/** FCC source oscillator/clock frequency in Hz. This example configures SYSOSC
  * configured for 24 MHz operation as the FCC source */
#define FCC_SRC_CLK_FREQ_IN_HZ                                        (24000000)

/** Accuracy of FCC source. Please refer to device datasheet to determine
 *  accuracy of source oscillator/clock */
#define FCC_SRC_CLK_P_M_TOL_PERC                                           (2.5)

/** FCC reference clock. This example configures LFXT which has operating of
 * 32768 Hz and the FCC reference clock */
#define FCC_REF_CLK_FREQ_IN_HZ                                           (32768)

/** Number of FCC monitoring periods. For this example the FCC is configured in
 * Rising-edge to Rising-edge and the number of monitorig period is 2
 * (DL_SYSCTL_FCC_TRIG_CNT_02) */
#define FCC_NUM_OF_MONITORING_PERIODS                                        (2)

/** The following defines are used to derived calculation to determine the
 * pass/fail range for FCC test */
#define SRC_CLK_FREQ_DEV     (FCC_SRC_CLK_FREQ_IN_HZ * FCC_SRC_CLK_P_M_TOL_PERC / 100)
#define MAX_SRC_CLK_FREQ_DEV (FCC_SRC_CLK_FREQ_IN_HZ + SRC_CLK_FREQ_DEV)
#define MIN_SRC_CLK_FREQ_DEV (FCC_SRC_CLK_FREQ_IN_HZ - SRC_CLK_FREQ_DEV)
#define EXPECTED_FCC_VAL_MIN ((MIN_SRC_CLK_FREQ_DEV * FCC_NUM_OF_MONITORING_PERIODS) / FCC_REF_CLK_FREQ_IN_HZ)
#define EXPECTED_FCC_VAL_MAX ((MAX_SRC_CLK_FREQ_DEV * FCC_NUM_OF_MONITORING_PERIODS) / FCC_REF_CLK_FREQ_IN_HZ)

/* clang-format on */

volatile uint32_t fccVal;

int main(void)
{
    SYSCFG_DL_init();

    DL_SYSCTL_startFCC();

    while (false == DL_SYSCTL_isFCCDone())
        ;

    fccVal = DL_SYSCTL_readFCC();

    /* Determine if measured FCC source periods are within expected range */
    if ((fccVal <= EXPECTED_FCC_VAL_MAX) && (fccVal >= EXPECTED_FCC_VAL_MIN)) {
        /* Test passed */
        DL_GPIO_clearPins(GPIO_LEDS_PORT,
            GPIO_LEDS_USER_LED_1_PIN | GPIO_LEDS_USER_TEST_PIN);
    }

    DL_SYSCTL_disableSleepOnExit();
    while (1) {
        __WFI();
    }
}
